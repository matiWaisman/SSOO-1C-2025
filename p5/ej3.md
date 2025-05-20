El codigo del driver es: 

```c
semaphore mutex;
int driver_init(){
    sema_init(mutex, 1);
    return 0;
}

int driver_read(int* udata){
    mutex.wait();
    while(IN(BTN_STATUS) && 0b1 != 0){}
    // Cuando estamos aca es porque efectivamente se presiono
    int numero_reinicio = 1
    out(BTN_STATUS, &numero_reinicio);
    mutex.signal();
    int res = BTN_PRESSED;
    copy_to_user(udata, &res, sizeof(res));
    return 1; // Porque leimos un byte
}
```