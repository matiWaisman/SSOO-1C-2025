## Punto A

Tenemos la balanza, el motor de la cinta transportadora y el beeper que trabajan todos en conjunto. 

La balanza tiene que tener un registro para escribir `MIN` y otro `MAX` para indicarle los valores `t1` y `t2` que funcionan como minimo. Tambien tiene que tener un registro de solo lectura `CURRENT_STATUS` que va a ser -1 si hubo un error, 0 si esta excedida de peso 1 si no hay nada, 2 si el valor esta entre min y max y 3 si está debajo de min pero es mayor a cero.

El motor tiene que tener un registro de escritura `CURRENT_PACE`. Si le pasamos un dos va a pasar a estar en modo rápido, si le pasamos un 1 va a estar en modo lento y si le pasamos un cero va a dejar de moverse.

El beeper tiene que tener un registro para escribir en caso de que se rompa algo y haya que llamar a los tecnicos. Lo llamo `BEP_WRITE`.

Vamos a tener que implementar "3 drivers", uno para poder comunicarnos con cada dispositivo. 

## Punto B

Programa de usuario: 

```c
int main(int argc, char** argv){
    int t1;
    int t2; 
    cin >> t1 >> t2; 

    int balanza = open("/dev/balanza");
    int motor = open("/dev/motor");
    int beeper = open("/dev/beeper");
    int ts[2];
    ts[0] = t1;
    ts[1] = t2;
    write(balanza, &ts, 8); // Me tengo que acordar de hacer un copy_from_user en el driver.
    while(1){
        // Asumo que el read de la balanza es bloqueante (lo voy a codear para que lo sea)
        int valor_balanza = read(balanza);
        if(valor_balanza < 0){ // Si hubo un error avisamos por el beeper y frenamos el motor
            write(motor, 0, 8);
            write(beeper, BROKEN_B, 8);
        }
        if(valor_balanza == 0){ // Si hay sobrepeso hacemos que la cinta pare y hacemos una alerta
            write(motor, 0, 8);
            write(beeper, EXCEEDED_WEIGHT, 8);
        }
        if(valor_balanza == 1){ // Si no hay nada paramos el motor
            write(motor, 0, 8);
        }
        if(valor_balanza == 2){ // Si el valor está entre min y max tiene que ir en lento (1)
            write(motor, 1, 8);
        }
        if(valor_balanza == 3){
            write(motor, 2, 8);
        }
    }
}
```

Ahora codeo los drivers. 

Vamos primero con el del beeper, como lo unico que necesito del beeper es cuando hago write hacer out al registro simplemente codeo el write, en el open, close load y unload no haria nada porque no hay ninguna variable interna a setear. 

```c
int driver_write(char* input, int size){
    char* buffer = kmalloc(size);
    copy_from_user(buffer, input, size);
    OUT(BEP_WRITE, buffer);
    kfree(buffer);
    return size;
}
```

Ahora codeo el del motor, mismo caso que el beeper. Como no hay ninguna estructura interna solo escribo el write. 

```c
int driver_write(char* input, int size){
    char* buffer = kmalloc(size);
    copy_from_user(buffer, input, size);
    OUT(CURRENT_PACE, buffer);
    kfree(buffer);
    return size;
}
```

Para el de la balanza si hace falta codear más que el write. Tambien hay que codear el read y un setup para setear las interrupciones. Como necesitamos que el read sea bloqueante tambien defino semaforos. 

```c
int cuanto_falta_esperar; 
semaphore mutex_var_espera; // Para manejar atomicamente la variable de arriba
semaphore semaforo_espera_resultado;
// Suponiendo que la unica aplicacion que va a usar el dispositivo es la que defini no pongo semaforos para manejar estas variables porque se van a setear una vez en el write y despues no se van a modificar mas, solo se van a leer.
int min; 
int max; 
void driver_load(){
    // Se ejecuta cuando se carga el driver en el kernel
    request_irq(CLOCK_IRQ, handler_clock);
    sema_init(mutex_var_espera, 1);
    sema_init(semaforo_espera_resultado, 0);
    cuanto_falta_esperar = 0; 
}

void driver_unload(){
    // Se ejecuta cuando se descarga el driver del kernel
    free_irq(CLOCK_IRQ);
}

int driver_write(int* input, int size){
    int * buffer = kmalloc(size);
    copy_from_user(buffer, input, size);
    min = buffer[0];
    max = buffer[1];
    kfree(buffer);
    OUT(MIN, min);
    OUT(MAX, max);
    return size;
}

int driver_read(int * buffer_usuario, int size){
    // Primero chequeo si tiene un valor invalido, si es asi seteamos las variables y nos colgamos hasta que nos despierten. 
    int valor_balanza_actual = IN(CURRENT_STATUS);
    if(valor_balanza_actual < 0){
        mutex_var_espera.wait();
        cuanto_falta_esperar = 11; // Lo inicializo en 11 por si estamos a mitad del ciclo del clock. 
        mutex_var_espera.signal();
        semaphore_espera_resultado.wait(); // De aca nos va a despertar la interrupcion, el valor que tenga la proxima vez va a ser el definitivo.
    }
    valor_balanza_actual = IN(CURRENT_STATUS);
    copy_to_user(buffer_usuario, valor_balanza_actual, size); // Abuso de notacion
    return 8; 
}

void handler_clock(){
    mutex_var_espera.wait();
    if(cuanto_falta_esperar > 0){ // Hay alguien esperando la interrupcion de clock para ver el valor de verdad de la balanza. 
        if(IN(CURRENT_STATUS) >= 0){ // Se arreglo
            cuanto_falta_esperar = 0; // Reinicio la variable para no tener signals sin nadie esperando
            mutex_var_espera.signal();
            mutex_espera_resultado.signal(); // Despierto al que estaba dormido
            return;
        }
        cuanto_falta_esperar--;
        if(cuanto_falta_esperar == 0){
            mutex_var_espera.signal();
            mutex_espera_resultado.signal(); // Despierto al que estaba dormido
            return;
        }
        
    }
    mutex_var_espera.signal();
}

```