## Punto A
A priori para no tener problemas de race condition de dos programas que quieran escribir en el disco a la vez y uno ya termino y lo quiere apagar mientras el otro lo quiere prender encierro toda la operacion dentro de un mutex.

```c
const int CTE_APAGAR = 0;
const int CTE_PRENDER = 1;
semaphore mutex;
int driver_init(){
    sema_init(mutex, 1);
    return 0;
}
// Agregue que me pasen el size del dato
int write(int sector, void *data, uint size){
    mutex.wait();
    // Primero checkeo si esta prendido o apagado
    if(in(DOR_STATUS) == 0){
        // Lo prendo y espero a que se pueda usar
        out(DOR_IO, CTE_PRENDER);
        sleep(50); 
    }
    // Pasamos los datos del usuario a la memoria del kernel
    // Pido la memoria necesaria 
    void * ptr_kernel = kmalloc(size);
    copy_from_user(ptr_kernel, data, size);
    int pista = sector/ cantidad_sectores_por_pista(); // Para calcular en que pista en particular tengo que escribir. Se deberia quedar con la parte entera
    int sector_dentro_de_pista = sector % cantidad_sectores_por_pista();
    // Hago que el brazo se mueva a la pista que queremos
    out(ARM, pista);
    // Hago busy waiting hasta que ponga el brazo en la pista que queremos 
    while(IN(ARM_STATUS) != 1){}
    // Una vez que estamos aca ya esta el brazo en la pista que queremos
    // Hacemos que sepa que tiene que escribir en el sector sector_dentro_de_pista
    OUT(SEEK_SECTOR, sector_dentro_de_pista);
    escribir_datos(ptr_kernel, size);
    // Hacemos busy waiting hasta que se escriba
    while(IN(DATA_READY) != 1){}
    // Libero la copia 
    kfree(ptr_kernel);
    // Apagamos el motor y despues dormimos hasta que efectivamente se apague
    out(DOR_IO, CTE_APAGAR);
    sleep(200);
    mutex.signal();
    return size; // Devolvemos la cantidad de bytes que escribimos
}
```

La idea de esta solucion es que cada proceso que quiera escribir algo en el disco lo haga uno a la vez cada uno encargandose y asegurandose de prender y apagar el disco. 

Una posible optimizacion puede ser si un proceso ve que esta prendido el semaforo espere hasta que el otro que lo este usando termine, y al terminar el proceso que estab escribiendo chequee si hay alguien esperando para escribir. Si hay alguien esperando no apaga el semaforo y despierta al proceso y si no hay nadie apaga el disco.

```c
const int CTE_APAGAR = 0;
const int CTE_PRENDER = 1;
int cantidad_esperando;
semaphore mutex_escribiendo;
semaphore mutex_esperando;
int driver_init(){
    cantidad_esperando = 0;
    sema_init(mutex_escribiendo, 1);
    sema_init(mutex_esperando, 1);
    return 0;
}
// Agregue que me pasen el size del dato
int write(int sector, void *data, uint size){
    // Primero me agrego a la "cola" de los que estan esperando
    mutex_esperando.wait();
    cantidad_esperando++;
    mutex_esperando.signal();

    mutex_escribiendo.wait();
    // Ahora hacemos lo mismo que en el otro codigo pero esperando a que no hay nadie ya trabajando sobre el disco
    // Primero checkeo si esta prendido o apagado
    if(in(DOR_STATUS) == 0){
        // Lo prendo y espero a que se pueda usar
        out(DOR_IO, CTE_PRENDER);
        sleep(50); 
    }
    // Pasamos los datos del usuario a la memoria del kernel
    // Pido la memoria necesaria 
    void * ptr_kernel = kmalloc(size);
    copy_from_user(ptr_kernel, data, size);
    int pista = sector/ cantidad_sectores_por_pista(); // Para calcular en que pista en particular tengo que escribir. Se deberia quedar con la parte entera
    int sector_dentro_de_pista = sector % cantidad_sectores_por_pista();
    // Hago que el brazo se mueva a la pista que queremos
    out(ARM, pista);
    // Hago busy waiting hasta que ponga el brazo en la pista que queremos 
    while(IN(ARM_STATUS) != 1){}
    // Una vez que estamos aca ya esta el brazo en la pista que queremos
    // Hacemos que sepa que tiene que escribir en el sector sector_dentro_de_pista
    OUT(SEEK_SECTOR, sector_dentro_de_pista);
    escribir_datos(ptr_kernel, size);
    // Hacemos busy waiting hasta que se escriba
    while(IN(DATA_READY) != 1){}
    // Libero la copia 
    kfree(ptr_kernel);
    mutex_escribiendo.signal();

    mutex_esperando.wait();
    // Si hay alguien esperando no apagamos el motor. A esta altura como minimo esta en 1, si es mayor o igual a 2 es porque hay al menos alguien esperando
    if(cantidad_esperando < 2){ 
        out(DOR_IO, CTE_APAGAR);
        sleep(200);
    }
    cantidad_esperando--;
    mutex_esperando.signal();
    return size; // Devolvemos la cantidad de bytes que escribimos
}
```

## Punto B
```c
const int CTE_APAGAR = 0;
const int CTE_PRENDER = 1;
int cantidad_esperando_escribir;
semaphore mutex_escribiendo;
semaphore mutex_esperando_escribir;
semaphore semaforo_ready;
semaphore semaforo_prendio;
semaphore mutex_esperando_a_prender; // Este semaforo es para manejar la variable alguien_esperando_a_prender. Si pudiera usar variables atomicas seria mas facil y no tendria 45 mil semaforos
bool alguien_esperando_a_prender;
int driver_init(){
    cantidad_esperando_escribir = 0;
    alguien_esperando_a_prender = false;
    irq_register(6, handler_ready);
    irq_register(7, handler_50);
    sema_init(mutex_escribiendo, 1);
    sema_init(mutex_esperando_escribir, 1);
    sema_init(semaforo_ready, 0);
    sema_init(semaforo_prendio, 0);
    sema_init(mutex_esperando_a_prender, 1);

    return 0;
}

int driver_remove(){
    free_irq(6);
    free_irq(7);
    return 0;
}

// En este caso no puede pasar como el ejercicio 4 que podemos dar un signal sin nadie esperando, asi que directamente lo hago. 
void handler_ready(){
    semaforo_ready.signal();
}

// En este caso no quiero prender el semaforo hasta no estar seguro que hay alguien esperandolo. Porque si no podria pasar de largo la proxima vez
void handler_50(){
    mutex_esperando_a_prender.wait();
    if(alguien_esperando_a_prender){
        if(IN(DOR_STATUS)){
            alguien_esperando_a_prender = false;
            semaforo_prendio.signal();
        }
    }
    mutex_esperando_a_prender.signal();
    return 0;
}

// Agregue que me pasen el size del dato
int write(int sector, void *data, uint size){
    // Primero me agrego a la "cola" de los que estan esperando
    mutex_esperando_escribir.wait();
    cantidad_esperando_escribir++;
    mutex_esperando_escribir.signal();

    mutex_escribiendo.wait();
    // Ahora hacemos lo mismo que en el otro codigo pero esperando a que no hay nadie ya trabajando sobre el disco
    // Primero checkeo si esta prendido o apagado
    if(in(DOR_STATUS) == 0){
        // Lo prendo y espero a que se pueda usar
        mutex_esperando_a_prender.wait();
        alguien_esperando_a_prender = true; 
        mutex_esperando_a_prender.signal();
        out(DOR_IO, CTE_PRENDER);
        semaforo_prendio.wait(); // Cuando nos desbloquiemos es que va a estar prendido
    }
    // Pasamos los datos del usuario a la memoria del kernel
    // Pido la memoria necesaria 
    void * ptr_kernel = kmalloc(size);
    copy_from_user(ptr_kernel, data, size);
    int pista = sector / cantidad_sectores_por_pista(); // Para calcular en qué pista en particular tengo que escribir
    int sector_dentro_de_pista = sector % cantidad_sectores_por_pista();
    // Hago que el brazo se mueva a la pista que queremos
    out(ARM, pista);
    // Espero al semáforo de Ready que se va a levantar cuando llegue la interrupción de que el brazo llegó a la pista
    semaforo_ready.wait();
    // Una vez que estamos acá ya está el brazo en la pista que queremos
    // Hacemos que sepa que tiene que escribir en el sector sector_dentro_de_pista
    OUT(SEEK_SECTOR, sector_dentro_de_pista);
    escribir_datos(ptr_kernel, size);
    // Espero al semáforo de Ready que se va a levantar cuando se escriba el dato
    semaforo_ready.wait();
    // Libero la copia 
    kfree(ptr_kernel);
    mutex_escribiendo.signal();

    mutex_esperando_escribir.wait();
    // Si hay alguien esperando no apagamos el motor. A esta altura como mínimo está en 1, si es mayor o igual a 2 es porque hay al menos alguien esperando
    if(cantidad_esperando_escribir < 2){ 
        out(DOR_IO, CTE_APAGAR);
        sleep(200);
    }
    cantidad_esperando_escribir--;
    mutex_esperando_escribir.signal();
    return size; // Devolvemos la cantidad de bytes que escribimos
}
```