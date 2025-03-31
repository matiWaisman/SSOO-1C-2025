#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void exec(const char *arg){
    int status;
    pid_t proceso_hijo = fork();
    if(proceso_hijo == 0){
        execl("/bin/sh", "sh", "-c", arg, (char *)NULL);
        // Sería mejor usar execvp que recibe una lista de argumentos. Pero uso execl como dice el manual que usa system.
        exit(EXIT_SUCCESS);
    }
    else{
        waitpid(proceso_hijo, &status, 0);
        if (WEXITSTATUS(status) != 0) exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Error: Se requiere un string como parámetro.\n");
        printf("Uso: %s <string>\n", argv[0]);
        return 1;
    }
    exec(argv[1]);
    return 0;
}