#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void handler_padre(){
    printf("Pong del padre con pid: %d\n", getpid());
}

void handler_hijo(){
    printf("Ping del hijo con pid: %d\n", getpid());
    kill(getppid(), SIGUSR1);
}

int main(){
    pid_t pid_hijo = fork();
    if(pid_hijo != 0){
        // Estamos en el padre
        sleep(1); // Meto un sleep para que el hijo setee el signal
        signal(SIGUSR1, handler_padre); // Defino el handler para responder al hijo 
        int status;
        char caracter;
        int sigo = 1;
        while(sigo){
            for(int i = 0; i < 3; i++){
                kill(pid_hijo, SIGUSR2);
                pause(); // Esperamos hasta que nos mande una señal el hijo. Despues de recibir la señal y ejecutar el hanlder vamos a la siguiente linea
            }
            printf("Queres seguir? [y/n]: ");
            scanf(" %c", &caracter);
            if(caracter != 'y'){
                sigo = 0;
            }
        }
        kill(pid_hijo, SIGTERM);
        exit(EXIT_SUCCESS);
    }
    else {
        // Estamos en el hijo 
        signal(SIGUSR2, handler_hijo);
        while(1){
            pause();
        }
    }
}

/*
Version alternativa usando un solo handler

void handler(){
    printf("Pong, %d \n", getpid());
}

int main(){
    char caracter;
    pid_t pid_hijo = fork();
    if(pid_hijo != 0){
        // Estoy en el padre
        sleep(1); // Meto un sleep para que el hijo setee el signal
        int contador = 0;
        while (contador < 3){
            printf("Ping, %d \n", getpid());
            kill(pid_hijo,SIGUSR1);
            sleep(1); // Le doy tiempo al hijo para que responda podria meter un sleep y hacer que el hijo le mande otra señal
            contador++;
            if(contador == 3){
                printf("Queres seguir? [y/n]: ");
                scanf(" %c", &caracter);
                if(caracter == 'y'){
                    contador = 0;
                }
            }
        } 
        kill(pid_hijo, SIGTERM); // Cuando termino mato al hijo   
    }
    else{
        // Estoy en el hijo
        signal(SIGUSR1, handler); // Si el proceso hijo recibe un SIGUSR1 lo handlea la funcion handler_señal
        while(1){
            pause();
        } // Hago que el proceso hijo espere hasta que le llegue una señal para handlearla
    }
    return 0;
}
*/