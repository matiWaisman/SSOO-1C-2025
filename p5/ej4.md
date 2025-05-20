La idea ahora que podemos usar una interrupcion es que si se quiere leer y no se presiono el boton que se quede colgado en un semaforo que inicializamos en cero. 

Para que no pase que se levante la interrupcion antes de que alguien lea, y la proxima vez que lea pueda pasar el semaforo de largo agrego el semaforo `mutex_esperando` y la variable `cantidad_esperando`

El codigo del driver es: 

```c
semaphore semaforo_read;
semaphore semaforo_int;
semaphore mutex;
semaphore mutex_esperando;
int cantidad_esperando;
int driver_init(){
    sema_init(semaforo_read, 0);
    sema_init(mutex, 1);
    sema_init(mutex_esperando, 1);
    irq_register(7, handler);
    int cantidad_esperando = 0;
    return 0;
}

int driver_remove(){
    free_irq(7);
    return 0;
}

void handler(){
    mutex_esperando.wait();
    if(cantidad_esperando > 0){
        // Hay alguien esperando asi que lo despertamos y decrementamos la cantidad esperando
        cantidad_esperando--;
        // Como sabemos que alguien lo presiono mandamos un signal para que si alguien estaba leyendo y quedo colgado lo lea
        semaforo_read.signal();
    }
    else{
        // Si no hay nadie esperando no hacemos nada
    }
    mutex_esperando.signal();
}

int driver_read(int* udata){
    mutex_esperando.wait();
    cantidad_esperando++;
    mutex_esperando.signal();

    semaforo_read.wait();
    mutex.wait();
    int numero_reinicio = 1
    out(BTN_STATUS, &numero_reinicio);
    mutex.signal();

    int res = BTN_PRESSED;
    copy_to_user(udata, &res, sizeof(res));
    return 1; // Porque leimos un byte
}
```