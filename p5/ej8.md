```c
semaphore mutex; // A priori como no queremos que hayan dos procesos imprimiendo a la vez encerramos toda la operacion de imprimir dentro de una zona protegida.
semaphore semaforo_imprimiendo;
int driver_init(){
    sema_init(mutex, 1);
    sema_init(semaforo_imprimiendo, 0);
    irq_register(HP_FINISHED_INT, handler_finished);
    return 0;
}

int driver_remove(){
    free_irq(HP_FINISHED_INT);
    return 0;
}

void handler_finished(){
    semaforo_imprimiendo.signal();
    return 0;
}

int driver_write(char *buffer, uint size){
    int intentos_tinta = 0; // Variable no compartida 
    mutex.wait();
    // Pasamos los datos del usuario a la memoria del kernel
    // Pido la memoria necesaria 
    char* buffer_kernel = kmalloc(size);
    // Copio el buffer del usuario al espacio del kernel
    copy_from_user(buffer_kernel, buffer, size);

    // Ahora le indico a la impresora donde y cuanto leer
    OUT(LOC_TEXT_POINTER, buffer_kernel);
    OUT(LOC_TEXT_SIZE, size);
    OUT(LOC_CTRL, START);
    // Ahora chequeamos si hay tinta, si hay nos dormimos en el semaforo hasta que termine de imprimir y si no hay nos dormimos hasta que llegue la interrupcion de que hay tinta. 
    if(IN(LOC_CTRL) == PRINTING && IN(LOC_STATUS) == BUSY){
        // Nos colgamos hasta que termine de imprimir
        semaforo_imprimiendo.wait();
    }
    else{
        // Hacemos busy waiting hasta que haya tinta 
        for(int i = 0; i < 4; i++){
            OUT(LOC_CTRL, START);
            if(IN(LOC_CTRL) == PRINTING && IN(LOC_STATUS) == BUSY){
                break;
            }
        }
        // Si despues de los 5 intentos totales sigue sin haber tinta devolvemos -1
        if(IN(LOC_CTRL) == PRINTING && IN(LOC_STATUS) == BUSY){
            semaforo_imprimiendo.wait();
        }
        else{
            return -1; // Especie de codigo de error.
        }  
    }
    // Libero la memoria 
    kfree(buffer_kernel);
    mutex.signal();
    return size;
}
```

Como los intentos para chequear si hay tinta o no estan acotados por 5, me parece que al final es mas rapido hacer busy waiting que colgarse de una interrupcion y el cambio de contexto. No pasa lo mismo con terminar de escribir que puede tardar mas tiempo. 