#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// No esta terminado
// Usar write en vez de printf y revisar el orden de los sleeps.
int main(){
    pid_t pid_julieta = fork();
    if(pid_julieta != 0){
        printf("Soy Juan \n");
        sleep(1);
        wait(NULL); // Aca deberia hacer un signal de algun tipo
        pid_t pid_jorge = fork();
        if(pid_jorge != 0){
            printf("Soy Jorge \n");
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }
    else{
        printf("Soy Julieta \n");
        pid_t pid_jennifer = fork();
        if(pid_jennifer != 0){
            printf("Soy Jennifer \n");
        }
        sleep(1);
        exit(EXIT_SUCCESS);
    }
}