#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };

int result_ready = 0;

// Falta terminarlo 

void ejecutarHijo(int i, int pipes[][2], int cantidad_procesos){
    int lugar_escritura_hijo_padre = i + cantidad_procesos;
    int pipe_padre_hijo[] = pipes[i];
    int pipe_hijo_padre[] = pipes[lugar_escritura_hijo_padre];
    int parametro_calculo;
    read(pipe_padre_hijo[READ], &parametro_calculo, sizeof(int));

    // Cierro todos los pipes que no voy a usar
    close(pipe_padre_hijo[WRITE]);
    close(pipe_padre_hijo[READ]);
    close(pipe_hijo_padre[READ]);

    // Cierro todos los pipes que no son mios 
    for(int j = 0; j < cantidad_procesos * 2; j++){
        if(j != lugar_escritura_hijo_padre && j != i){
            close(pipes[j][READ]);
            close(pipes[j][WRITE]);
        }
    }


    signal(SIGCHLD, handler_termino_nieto); // Defino el handler que se va a encargar de saber cuando termina el nieto

    int pipe_nieto_hijo[2];
    pipe(pipe_nieto_hijo);
    pid_t pid_nieto = fork();

    if(pid_nieto != 0){
        // Soy el hijo
        while(1){
            if(result_ready){
                dup2(pipe_nieto_hijo[READ], pipe_hijo_padre[WRITE]);
                close(pipe_hijo_padre[WRITE]);
                close(pipe_nieto_hijo[READ]);
                exit(EXIT_SUCCESS);
            }
        }
    }
    else{
        // Soy el nieto
        close(pipe_nieto_hijo[READ]);
        int resultado = calcular(parametro_calculo);
        write(pipe_nieto_hijo[WRITE], &resultado, sizeof(resultado));
        close(pipe_nieto_hijo[WRITE]);
        exit(EXIT_SUCCESS);
    }

}

void handler_termino_nieto(){
    wait(NULL); // Para que no quede zombie el nieto
    result_ready = 1;
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Debe ejecutar con la cantidad de hijos como parámetro\n");
        return 0;
    }

    int N = atoi(argv[1]); // Pasa a numero entero
    int pipes[N * 2][2];

    // Crear pipes
    for (int i = 0; i < N * 2; i++) {
        pipe(pipes[i]);
    }

    // Crear hijos
    for (int i = 0; i < N; i++) {
        int pid = fork();
        if (pid == 0) {
            ejecutarHijo(i, pipes, N);
            return 0;
        } else {
            int numero = dameNumero(pid);
            write(pipes[i][WRITE], &numero, sizeof(numero));
        }
    }

    int cantidadTerminados = 0;
    char hijoTermino[N];
    for (int i = 0; i < N; i++) {
        hijoTermino[i] = 0;
    }

    // Esperar la finalización de los hijos
    while (cantidadTerminados < N) {
        for (int i = 0; i < N; i++) {
            if (hijoTermino[i]) {
                continue;
            }

            char termino = 0;
            write(pipes[i][WRITE], &termino, sizeof(termino)); 
            read(pipes[N + i][READ], &termino, sizeof(termino));

            if (termino) {
                int numero, resultado;
                read(pipes[N + i][READ], &numero, sizeof(numero));
                read(pipes[N + i][READ], &resultado, sizeof(resultado));

                informarResultado(numero, resultado);
                hijoTermino[i] = 1;
                cantidadTerminados++;
            }
        }
    }

    wait(NULL);
    return 0;
}


