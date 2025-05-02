#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };

// Debe ejecutar "ls -al"
void ejecutar_hijo_1(int pipe_fd[]) {
    close(pipe_fd[READ]); // Cierro la punta de lectura porque no voy a escribir
    dup2(pipe_fd[WRITE], STDOUT_FILENO);
    char *args[] = {"ls", "-al", NULL};
    execvp("ls", args);
    // Si estamos aca abajo es porque fallo el exec. Asi que cerramos el pipe y hacemos exit failure
    close(pipe_fd[WRITE]); // Cierro
    exit(EXIT_FAILURE);
}

// Debe ejecutar "wc -l"
void ejecutar_hijo_2(int pipe_fd[]) {
    close(pipe_fd[WRITE]); // Cierro la punta de escritura porque no lo voy a usar
    dup2(pipe_fd[READ], STDIN_FILENO);
    char *args[] = {"wc", "-l", NULL};
    execvp("wc", args);
    // Si estamos aca abajo es porque fallo el exec. Asi que cerramos el pipe y hacemos exit failure
    close(pipe_fd[READ]); 
    exit(EXIT_FAILURE);
}

int main(){
    int fd[2]; // fd[0] lectura fd[1] escritura
    pipe(fd);
    pid_t pid_hijo_2;
    pid_t pid_hijo_1 = fork();
    if(pid_hijo_1 == 0){
        ejecutar_hijo_1(fd);
    }
    else{
        pid_hijo_2 = fork();
        if(pid_hijo_2 == 0){
            ejecutar_hijo_2(fd);
        }
        
    }
    close(fd[READ]);
    close(fd[WRITE]);

    waitpid(pid_hijo_1, NULL, 0);
    waitpid(pid_hijo_2, NULL, 0);
    exit(EXIT_SUCCESS);
}


