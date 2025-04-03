#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };

// Esta lleno de codigo repetido. Habria que refactorizarlo
// El 53 es un parche pq si no escribe hasta 47
// Diria que el comportamiento de mensajes es igual al de las funciones ficticias bsend y breceive porque al irse bloqueando los procesos se coordina la escritura y lectura

void padre(int fd_p_h1[], int fd_h2_p[], pid_t pid_hijo_1, pid_t pid_hijo_2){
    close(fd_p_h1[READ]);
    close(fd_h2_p[WRITE]);
    int valor_a_enviar = 0;
    int proximo_valor_a_enviar = 3;
    while(proximo_valor_a_enviar <= 53){
        printf("Padre escribe en el pipe a Hijo 1 el valor %d \n", valor_a_enviar);
        write(fd_p_h1[WRITE], &valor_a_enviar, sizeof(int));
        //printf("Padre espera leer el dato que le vaya a pasar el hijo 2 \n");
        read(fd_h2_p[READ], &valor_a_enviar, sizeof(int));
        valor_a_enviar++;
        proximo_valor_a_enviar += 3;
    }
    close(fd_p_h1[WRITE]);
    close(fd_h2_p[READ]);
    waitpid(pid_hijo_1, NULL, 0);
    waitpid(pid_hijo_2, NULL, 0);
}

void hijo_2(int fd_h1_h2[], int fd_h2_p[]){
    close(fd_h2_p[READ]);
    close(fd_h1_h2[WRITE]);
    int valor_a_enviar = 2;
    int proximo_valor_a_enviar = 5;
    while(proximo_valor_a_enviar <= 53){
        //printf("Hijo 2 espera leer el dato que le vaya a pasar el hijo 1 \n");
        read(fd_h1_h2[READ], &valor_a_enviar, sizeof(int));
        valor_a_enviar++;
        proximo_valor_a_enviar += 3;
        printf("Hijo 2 escribe en el pipe a Padre el valor %d \n", valor_a_enviar);
        write(fd_h2_p[WRITE], &valor_a_enviar, sizeof(int));
    }
    close(fd_h2_p[WRITE]);
    close(fd_h1_h2[READ]);
    exit(EXIT_SUCCESS);
}

void hijo_1(int fd_h1_h2[], int fd_p_h1[]){
    close(fd_h1_h2[READ]);
    close(fd_p_h1[WRITE]);
    int valor_a_enviar = 1;
    int proximo_valor_a_enviar = 4;
    while(proximo_valor_a_enviar <= 53){
        //printf("Hijo 1 espera leer el dato que le vaya a pasar el padre \n");
        read(fd_p_h1[READ], &valor_a_enviar, sizeof(int));
        valor_a_enviar++;
        proximo_valor_a_enviar += 3;
        printf("Hijo 1 escribe en el pipe a Hijo 2 el valor %d \n", valor_a_enviar);
        write(fd_h1_h2[WRITE], &valor_a_enviar, sizeof(int));
    }
    close(fd_h1_h2[WRITE]);
    close(fd_p_h1[READ]);
    exit(EXIT_SUCCESS);
}

int main(){
    int fd_p_h1[2]; // Pipe para que padre le mande mensajes al hijo 1
    int fd_h1_h2[2]; // Pipe para que Hijo 1 le mande mensajes al hijo 2
    pipe(fd_p_h1);
    pipe(fd_h1_h2);
    pid_t pid_hijo_1 = fork();
    if(pid_hijo_1 != 0){
        // Estoy en el padre
        int fd_h2_p[2]; // pipe para que el hijo 2 se comunique con padre
        pipe(fd_h2_p);
        pid_t pid_hijo_2 = fork();
        if(pid_hijo_2 != 0){
            // Estoy en el padre
            padre(fd_p_h1, fd_h2_p, pid_hijo_1, pid_hijo_2);
        }
        else{
            // Estoy en el hijo 2
            hijo_2(fd_h1_h2, fd_h2_p);
        }
    }
    else{
        hijo_1(fd_h1_h2, fd_p_h1);
    }
    return 0;
}


