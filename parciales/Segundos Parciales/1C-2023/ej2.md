El sensor va a tener que tener tres registros de escritura. Uno llamado `MIN_TEMPERATURE`, otro `MAX_TEMPERATURE` y otro `CLOCK_TIMER`. Los primeros dos sirven para armar el rango de temperatura para el cual apagar, encender o mantener el ventilador como estaba. El ultimo registro sirve para setear sobre cuantos segundos es el promedio de temperatura. 

El sensor va a tener un registro de lectura `CURRENT_AVERAGE` que nos dice -1 si el promedio de la temperatura es menor al minimo, cero si esta dentro del rango y 1 si esta por arriba del rango. 

El sensor tambien va a tener una interrupcion `SENSOR_CLOCK` cada vez que pasen `CLOCK_TIMER` segundos luego de setearlo. 

El ventilador va a tener un registro de escritura `STATUS`. Si escribimos uno el ventilador se va a prender, si escribimos cero se va a apagar. 

Defino primero el programa de usuario que va a manejar la temperatura de la habitacion. 

```c
int main(int argc, char** argv){
    int buffer = malloc(3 * sizeof(int)); // Reservo la memoria que le voy a pasar al driver para setear los valores del sensor. 
    // Pasan primero la temperatura minima, segundo la maxima y tercero el tiempo. 
    int buffer[0] = atoi(argv[1]);
    int buffer[1] = atoi(argv[2]);
    int buffer[2] = atoi(argv[3]); 

    int sensor = open("/dev/sensor");
    int ventilador = open("/dev/ventilador");

    write(sensor, buffer, sizeof(buffer));

    while(1){
        int promedio_actual; 

        read(sensor, &promedio_actual, sizeof(promedio_actual));

        if(promedio_actual == -1){
            write(ventilador, 0, sizeof(int));
        }
        if(promedio_actual == 1){
            write(ventilador, 1, sizeof(int));
        }
    }
}
```

Ahora defino el driver del ventilador. Como asumo que solo va a interactuar con el ventilador nuestro programa y no hace falta definir ni variables ni estructuras de control para el driver solo escribo el write: 

```c
int driver_write(int * user_input, int size){
    int* buffer = kmalloc(size);
    copy_from_user(buffer, user_input, size);
    OUT(STATUS, buffer[0]);
    kfree(buffer);
    return size;
}
```

Ahora defino el driver del sensor. Para este si que vamos a necesitar un setup para definir un semaforo. El semaforo lo vamos a definir para la primer pregunta del promedio de temperatura, como todavia se pueden no haber cumplido los segundos necesarios para preguntarse el promedio de la temperatura hay que bloquearse esperando la primera vez.

```c
semaphore sem;

void driver_load(){
    sema_init(sem, 0);
    request_irq(CLOCK_TIMER, handler);
}

void driver_unload(){
    free_irq(CLOCK_TIMER);
}


int driver_write(int * user_input, int size){
    int* buffer = kmalloc(size);
    copy_from_user(buffer, user_input, size);
    OUT(MIN_TEMPERATURE, buffer[0]);
    OUT(MAX_TEMPERATURE, buffer[1]);
    OUT(CLOCK_TIMER, buffer[2]);
    kfree(buffer);
    return size;
}

int driver_read(int * user_buffer, int size){
    sem.wait();
    // Si estamos aca abajo es porque se registro al menos una lectura
    int promedio = IN(CURRENT_AVERAGE);
    copy_to_user(user_buffer, &promedio, sizeof(promedio));
    return size; 
}

void handler(){
    sem.signal();
}
```