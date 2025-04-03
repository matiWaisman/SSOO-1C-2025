#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

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