El esquema que voy a hacer va a ser: 
El proceso padre es el denominado "primer proceso". Este lee la cadena de `stdin`, crea 3 pipes. Uno para hablar con el hijo 1 que es el "segundo proceso" un pipe para que el hijo 1 hable con el hijo 2, que es el "tercer proceso", y por este pipe el "proceso 1" le va a mandar al "proceso 3" el string original. 

```c
void hijo1(int fd_padre_hijo1[2], int fd_padreYHijo1_hijo2[2], int length_cadena){
    // Cierro todos los extremos de los pipes que no voy a usar 
    close(fd_padre_hijo1[WRITE]);
    close(fd_padreYHijo1_hijo2[READ]);

    // Leo el string 
    char buffer[length_cadena]; 
    read(fd_padre_hijo1[READ], buffer, legth_cadena);

    char* invertida = invertir(buffer);

    write(fd_padreYHijo1_hijo2[WRITE], invertida, length_cadena);
    // Cierro los pipes y termino el proceso
    close(fd_padre_hijo1[READ]);
    close(fd_padreYHijo1_hijo2[WRITE]);
    exit(EXIT_SUCCESS);
}

void hijo2(int fd_padre_hijo1[2], int fd_padreYHijo1_hijo2[2], int length_cadena){
    // Cierro los extremos de los pipes que no voy a usar.
    close(fd_padre_hijo1[WRITE]);
    close(fd_padre_hijo1[READ]);
    close(fd_padreYHijo1_hijo2[WRITE]);
    char buffer_string_original[length_cadena];
    char buffer_string_reverso[length_cadena];
    // Como los pipes son fifo y escribo antes de forkear si o si primero va a estar el original. Igual no importa el orden, simplemente hay que compararlos
    read(fd_padreYHijo1_hijo2[READ], buffer_string_original, length_cadena);
    read(fd_padreYHijo1_hijo2[READ], buffer_string_reverso, length_cadena);

    if (memcmp(buffer_string_original, buffer_string_reverso, length_cadena) == 0) {
        printf("La cadena es un palíndromo.\n");
    } else {
        printf("La cadena NO es un palíndromo.\n");
    }
    // Cierro el pipe y hago exit
    close(fd_padreYHijo1_hijo2[READ]);
    exit(EXIT_SUCCESS);

}
// El primer parametro es la cadena que queremos checkear si es palindromo
int main(int argc, char **argv){
    char* cadena = argv[1];
    int length_cadena = strlen(cadena);
    int fd_padre_hijo1[2]; // En este pipe el padre le escribe el original al hijo 2
    int fd_padreYHijo1_hijo2[2]; // En este pipe el hijo 2 le escribe al 3 la cadena invertida y el padre le escribe la original.
    pipe(fd_padre_hijo1);
    pipe(fd_padreYHijo1_hijo2);
    // Les escribo a los hijos por los pipes las cadenas
    write(fd_padre_hijo1[WRITE], cadena, length_cadena);
    write(fd_padreYHijo1_hijo2[WRITE], cadena, length_cadena);
    // Hago los forks
    pid_t pid_hijo_1 = fork();
    if(pid_hijo_1 == 0){
        hijo1(fd_padre_hijo1, fd_padreYHijo1_hijo2, length_cadena);
    }
    else{
        // Estamos en el padre 
        pid_t pid_hijo_2 = fork();
        if(pid_hijo_2 == 0){
            hijo2(fd_padre_hijo1, fd_padreYHijo1_hijo2, length_cadena);
        }
    }
    // Waiteo a los dos hijos para que no queden zombies y para no terminar el proceso padre antes de los hijos
    wait(NULL);
    wait(NULL);

    return 0;
}
```