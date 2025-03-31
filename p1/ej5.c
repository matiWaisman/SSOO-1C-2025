#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ej5.h"

void abraham() {
    pid_t proceso_homero = fork(); // Preguntar el tema de los nombres para que sea mas claro
    if (proceso_homero != 0){
        printf("Hola soy Abraham mi pid es: %d \n", getpid());
        wait(NULL); // Esperar a que termine Homero
        exit(EXIT_SUCCESS);
    }
    else{
        homero();
    }
}
void homero() {
    printf("Hola soy Homelo Chino, mi pid es: %d y el de mi papa es: %d \n", getpid(), getppid());
    pid_t proceso_bart = fork();
    if (proceso_bart == 0){
        bart();
    }
    else
    {
        wait(NULL); // Esperar a que termine Bart
        pid_t proceso_lisa = fork();
        if (proceso_lisa == 0){
            lisa();
        }
        else{
            wait(NULL); // Esperar a que termine lisa
            pid_t proceso_maggie = fork();
            if (proceso_maggie == 0){
                maggie();
            }
        }
    }
    wait(NULL); // Esperar a que termine Maggie
    exit(EXIT_SUCCESS);
}

void maggie() {
    printf("Hola soy Maggie, mi pid es: %d y el de mi papa es: %d \n", getpid(), getppid());
    exit(EXIT_SUCCESS);
}

void lisa() {
    printf("Hola soy Lisa, mi pid es: %d y el de mi papa es: %d \n", getpid(), getppid());
    exit(EXIT_SUCCESS);
}

void bart() {
    printf("Hola soy Bart, mi pid es: %d y el de mi papa es: %d \n", getpid(), getppid());
    exit(EXIT_SUCCESS);
}

int main(){
    // Faltaria checkeo de errores de los forks
    abraham();
    return 0;
}