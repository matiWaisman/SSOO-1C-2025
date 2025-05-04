Por la aclaracion escrita en lapiz en la consigna entiendo que el largo de la palabra a adivinar coincide con la cantidad de procesos. Por lo que cada proceso va a modificar un caracter exclusivo. 

Asumo que al programa le pasan en orden, primero una palabra inicial de 5 caracteres, segundo la cantidad de procesos `n` y tercero `t`. 

La palabra inicial se podria hacer random. 
```c
int N;
int t; 
int lugar_hijo; // Para que el hijo sepa cual es su numero de proceso

void proceso_hijo(int pipes[][2]){
    // Primero cierro todos los pipes que no voy a usar. 
    // El pipe lugar_hijo no voy a cerrar el extremo de lectura y el lugar_hijo + 1 no voy a cerrar el de escritura
    close(pipes[lugar_hijo][WRITE]);
    close(pipes[lugar_hijo + 1][READ]);
    for(int i = 0; i < N; i++){
        if(i != lugar_hijo && i != lugar_hijo + 1){
            close(pipes[i][WRITE]);
            close(pipes[i][READ]);
        }
    }
    // A partir de aca me cuelgo en un while tratando de leer del pipe y modificando mi caracter. El chiste es que cuando terminen las rondas me quedo colgado en el read y mi padre me mata :(
    char buffer[N];
    while(1){
        int status = read(pipes[lugar_hijo][READ], buffer, N);
        // Si me llega un EOF porque nadie mas me va a escribir salgo del ciclo porque se termino el juego. 
        if(status == 0){
            break;
        }
        char* palabra_modificada = modificar_palabra(&buffer, lugar_hijo);
        write(pipes[lugar_hijo + 1][WRITE], palabra_modificada, N);
    }
    // Cierro mi escritura para que haga efecto cascada y el siguiente hijo tambien termine 
    close(pipes[lugar_hijo + 1][WRITE]); 
    exit(EXIT_SUCCESS);

}

int main(int argc, char **argv){
    char* palabra = argv[1];
    N = atoi(argv[2]);
    t = atoi(argv[3]);
    pid_t pids[N]; 
    // Inicializo N + 1 pipes. Cada proceso i-esimo va a leer del i-esimo pipe y va a escribir en el i + 1. Del i + 1 va a leer el padre para enterarse del resultado. 
    int pipes[N + 1][2];
    for(int i = 0; i < N+1; i++){
        pipe(pipes[i]);
    }
    // Ahora arranco a forkear. 
    for(int i = 0; i < N; i++){
        lugar_del_hijo = i;
        pid_t pid_hijo = fork();
        if(pid_hijo == 0){
            // Estamos en el hijo 
            proceso_hijo(pipes);
        }
        else{
            // Estamos en el padre
            // No hay que hacer nada mas que guardar el pid en esta rama del if. Cuando terminemos de crear los procesos ahi si empiezo a mandar acorde a las rondas.
            pids[i] = pid;
        }
    }
    // Ahora que ya hicimos los forks cierro todos los pipes que no se van a usar 
    for(int j = 1; j < N; j++){
        close(pipes[j][READ]);
        close(pipes[j][WRITE]);
    }
    // El primero voy a escribir y en el ultimo voy a leer asi que no cierro esos extremos
    close(pipes[0][READ]);
    close(pipes[N][WRITE]);
    // Arranca el juego
    bool gane = false;
    for(int i = 0; i < t; i++){
        write(pipes[0][WRITE], palabra, strlen(palabra));
        read(pipes[N][READ], palabra, strlen(palabra));
        if(adivinarWordle(palabra)){
            gane = true; 
            break;
        }
    }
    if(gane){
        printf("Ganamos! La palabra del dia era: %s\n", palabra);
    }
    else{
        printf("Perdimos :(\n");
    }
    // Ahora cierro la escritura al primer hijo para que le llegue un EOF y propague el EOF en todos los hijos haciendo que terminen todos 
    close(pipes[0][WRITE]);
    // Ahora waiteo a todos los hijos para que terminen y no queden zombies 
    for(int i = 0; i < N; i++){
        wait(NULL);
    }
    return 0;
}


```