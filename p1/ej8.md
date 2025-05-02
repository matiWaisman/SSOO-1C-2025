```c
int main(int argc, char const *argv[]) {
    int dato = 0;
    pid_t pid = fork();

    // Si no hay error, pid vale 0 para el hijo
    // y el valor del process id del hijo para el padre
    if (pid == -1) {
        exit(EXIT_FAILURE); // Si es -1, hubo un error
    } 
    else if (pid == 0) {
        for (int i = 0; i < 3; i++) {
            dato++;
            printf("Dato hijo: %d\n", dato);
        }
    } 
    else {
        for (int i = 0; i < 3; i++) {
            printf("Dato padre: %d\n", dato);
        }
    }

    exit(EXIT_SUCCESS); // Cada uno finaliza su proceso
}
```

Los prints del proceso hijo van a ser:

Dato Hijo: 1
Dato Hijo: 2
Dato Hijo: 3

En cambio los del padre van a ser: 

Dato Padre: 0
Dato Padre: 0
Dato Padre: 0

Esto pasa porque al hacer el fork ambos procesos van a arrancar con dato valiendo cero, pero la copia en memoria del valor dato es distinta, porque los procesos no comparten memoria. 
Si se ejecuta primero el hijo se va a incrementar el valor de dato pero cuando se ejecute el padre como la memoria no se comparte entre procesos entonces dato va a seguir valiendo cero. 

Tampoco podemos asegurar el orden de los prints porque depende del orden en el que los ejecute el scheduler. 
