El codigo del driver es: 

```c
semaphore mutex;
int driver_init(){
    sema_init(mutex, 1);
    return 0;
}

int driver_write(int* udata){
    mutex.wait();
    OUT(CHRONO_CTRL, CHRONO_RESET);
    mutex.signal();
    return 0;
}

int driver_read(int* udata){
    mutex.wait();
    int current_time = IN(CHRONO_CURRENT_TIME);
    copy_to_user(udata, &current_time, sizeof(current_time))
    mutex.signal();
    return sizeof(current_time);
}
```

El codigo de una aplicación de usuario que la utiliza puede ser: 
```c
int main(int argc, char* argv[]){
    int cronometro = open("/dev/cronometro", "rw");
    int numero;
    int tiempo;
    while(1){
        printf("Queres leer el cronometro (0) o reiniciarlo (1)? ");
        scanf("%d", &numero);
        if(numero == 0){
            read(cronometro, &tiempo);
            printf("El cronometro midio %d segundos \n");
        }
        if(numero == 1){
            write(cronometro, 0);
        }
        else{
            break;
        }
    }
    close("/dev/cronometro");
    exit(EXIT_SUCCESS);
}
```