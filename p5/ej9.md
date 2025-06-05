```c
const int N_APLICACIONES_POSIBLES = 3;
char input_mem[3][100];
char buffer_lectura[3][1000];
atomic_int buffer_start[3];
atomic_int buffer_end[3];
bool procesos_activos[3];
semaphore mutex_estructuras_internas;
semaphore mutex_interaccion_con_teclado;
semaphore mutex_esperando_datos[3];
int cantidad_datos_esperando[3]; // Indica la cantidad de datos que esta esperando que le lleguen para leer

void driver_load() {
    // Se corre al cargar el driver al kernel.
    sema_init(mutex_estructuras_internas, 1);
    sema_init(mutex_interaccion_con_teclado, 1);
    // Inicializo procesos_activos todos en falso y los starts y ends en cero
    for(int i = 0; i < N_APLICACIONES_POSIBLES; i++){
        procesos_activos[i] = 0;
        buffer_start[i] = 0;
        buffer_end[i] = 0;
        sema_init(mutex_esperando_datos[i], 0);
        cantidad_datos_esperando[i] = 0;
    }
    irq_register(IRQ_KEYB, handler_keyb);
    // Mapeo las direcciones del dispositivo a memoria del kernel
    mem_map(INPUT_MEM_0, input_mem[0], 100);
    mem_map(INPUT_MEM_1, input_mem[1], 100);
    mem_map(INPUT_MEM_2, input_mem[2], 100);
    return 0;
}

void driver_unload() {
    // Se corre al eliminar el driver del kernel.
    free_irq(IRQ_KEYB);
    // Desmapeo la memoria
    for(int i = 0; i < N_APLICACIONES_POSIBLES; i++){
        mem_unmap(input_mem[i]);
    }
    return 0;
}

// Capaz estaria mejor encerrar todo en ambos mutexs. Hasta que no podes interactuar con el teclado y actualizar las estructuras internas no podes.
int driver_open() {
    // Debe conectar un proceso, asignándole un ID y retornándolo,
    // o retornando -1 en caso de falla.
    int res = -1;
    mutex_estructuras_internas.wait();
    for(int i = 0; i < N_APLICACIONES_POSIBLES; i++){
        if(!procesos_activos[i]){
            procesos_activos[i] = 1;
            res = i;
            break;
        }
    }
    // Si no encontramos un lugar devolvemos -1
    if(res == -1){
        mutex_estructuras_internas.signal();
        return res;
    }
    mutex_estructuras_internas.signal();
    // Le indico al teclado que se conecto una aplicacion al teclado
    mutex_interaccion_con_teclado.wait();
    OUT(KEYB_REG_AUX, res);
    OUT(KEYB_REG_STATUS, APP_UP);
    mutex_interaccion_con_teclado.signal();
    return res; 
}

// Capaz estaria mejor encerrar todo en ambos mutexs. Hasta que no podes interactuar con el teclado y actualizar las estructuras internas no podes.
void driver_close(int id) {
    // Debe desconectar un proceso dado por parámetro.
    // Le avisamos al teclado que se desconecto una aplicacion
    mutex_interaccion_con_teclado.wait();
    OUT(KEYB_REG_AUX, id);
    OUT(KEYB_REG_STATUS, APP_DOWN);
    mutex_interaccion_con_teclado.signal();
    // Nos guardamos en nuestra estructura interna que se libero un lugar. 
    // Me parece mejor primero avisarle al teclado y despues actualizar nuestras estructuras internas porque si no 
    // Podria pasar una race condition en la que en las estructuras internas se libero el lugar pero el teclado no sabe 
    // Que se desconecto un cliente y le estan pidiendo que conecte a otro. 
    mutex_estructuras_internas.wait();
    procesos_activos[id] = 0;
    buffer_start[id] = 0;
    buffer_end[id]  = 0;
    sema_init(mutex_esperando_datos[id], 0);
    cantidad_datos_esperando[id] = 0;
    mutex_estructuras_internas.signal();
    return 0;
}

int driver_read(int id, char* buffer, int length) {
    mutex_estructuras_internas.wait();
    if(procesos_activos[id] == 0){ // Si me piden leer de un id que no esta conectado devuelvo cero, o se podria devolver un codigo de error como -1
        mutex_estructuras_internas.signal();
        return 0;
    }
    mutex_estructuras_internas.signal();

    mutex_interaccion_con_teclado.wait();
    int cantidad_bytes_escritos = get_buffer_length(id);
    if(cantidad_bytes_escritos >= length){ // Esta disponible la cantidad de informacion que queremos leer
        copy_from_buffer(id, buffer, length); // Entiendo que esto actualiza buffer_end[id]
        mutex_interaccion_con_teclado.signal();
        // Actualizamos el start. Falta completar
        //mutex_estructuras_internas.wait();
        //for(int i = 0; i < length; i++){
        //    buffer_end[i].getAndInc();
        //}
        //mutex_estructuras_internas.signal();
    }
    else{ // Nos tenemos que bloquear hasta que esten disponibles nuestros caracteres
        mutex_interaccion_con_teclado.signal();
        mutex_estructuras_internas.wait();
        cantidad_datos_esperando[id] = length - cantidad_bytes_escritos;
        mutex_estructuras_internas.signal();
        mutex_esperando_datos[id].wait(); // Esperamos hasta que alguien nos resuelva lo que leimos y nos despierte. Va a actualizar cantidad_datos_esperando[id] la interrupcion cuando nos salve
        mutex_interaccion_con_teclado.wait();
        copy_from_buffer(id, buffer, length);
        mutex_interaccion_con_teclado.signal();
    }
    
    return length; 
}

int driver_write(char* input, int size, int proceso) {
    copy_from_user(input_mem[proceso], input, size);
    return size;
}

void handler_keyb(){
    mutex_interaccion_con_teclado.wait();
    int datos = IN(KEYB_REG_DATA);
    mutex_interaccion_con_teclado.signal();
    int keycode = datos & 0x3FFF;
    int identificador = (datos & 0xC000) >> 14;
    char ascii = keycode2ascii(keycode);
    mutex_interaccion_con_teclado.wait();
    if (identificador == 0){ // En este caso le tenemos que escribir a todos los procesos
        bool pude = write_to_all_buffers(ascii); 
        if(!pude){
            OUT(KEYB_REG_CONTROL, READ_FAILED);
            mutex_interaccion_con_teclado.signal();
            return;
        }
        else{
            OUT(KEYB_REG_CONTROL, READ_OK);
        }
    }
    else{ // En este solo a identificador - 1
        bool pude = write_to_buffer(identificador - 1, ascii);
        if(!pude){
            OUT(KEYB_REG_CONTROL, READ_FAILED);
            mutex_interaccion_con_teclado.signal();
        }
        else{
            OUT(KEYB_REG_CONTROL, READ_OK);
        }
    }
    mutex_interaccion_con_teclado.signal();
    mutex_estructuras_internas.wait();
    if (identificador == 0){ // Si hay que escribir a todos
        for(int i = 0; i < N_APLICACIONES_POSIBLES; i++){
            buffer_end[i].getAndInc(); // Le sumamos uno de manera atomica al buffer end
            if(cantidad_datos_esperando[i] > 0){ // Si esta esperando datos por leer
                cantidad_datos_esperando[i]--;
                if(cantidad_datos_esperando[i] == 0){
                    mutex_esperando_datos[i].signal();
                }
            }
        }
    }
    else{ // Si hay que escribirle a uno solo
        buffer_end[identificador - 1].getAndInc(); // Le sumamos uno de manera atomica al buffer end
        if(cantidad_datos_esperando[identificador - 1] > 0){ // Si esta esperando datos por leer
            cantidad_datos_esperando[identificador - 1]--;
            if(cantidad_datos_esperando[identificador - 1] == 0){
                mutex_esperando_datos[identificador - 1].signal();
            }
        }
    }
    mutex_estructuras_internas.signal();
}
```