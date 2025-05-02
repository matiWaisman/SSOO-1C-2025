#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <math.h>
#include <sys/wait.h>

enum { READ, WRITE }; 

int armar_socket_cliente(int socket_servidor){
    int socket_cliente;

    struct sockaddr_un addr_cliente;

    unsigned int length_cliente = sizeof(addr_cliente);

    socket_cliente = accept(socket_servidor, (struct sockaddr *) &addr_cliente, &length_cliente);

    return socket_cliente;
}

int armar_socket_servidor(){
    int socket_servidor; 

    struct sockaddr_un addr_servidor;
    
    addr_servidor.sun_family = AF_UNIX;
    strcpy(addr_servidor.sun_path, "/tmp/unix_socket");
    unlink(addr_servidor.sun_path);

    socket_servidor = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(socket_servidor, (struct sockaddr *) &addr_servidor, (unsigned int)sizeof(addr_servidor));
    listen(socket_servidor, SOMAXCONN);

    printf("Servidor conectado esperando conexiones\n");
    return socket_servidor;
}

void proceso_hijo(int fd[2], int num, int desde, int hasta){
    close(fd[READ]); 
    int res = 0;
    for(int i = desde; i < hasta; i++){
        res = res || (num % i == 0);
    }
    write(fd[WRITE], &res, sizeof(res));
    close(fd[WRITE]);
    exit(EXIT_SUCCESS);
}

int leer_pipes(int fds[][2]){
    int res = 0;
    for(int i = 0; i < 3; i++){
        int res_actual;
        read(fds[i][READ], &res_actual, sizeof(res_actual));
        res = res || res_actual;
    }
    return res;
}

void wait_hijos(){
    for (int i = 0; i < 3; i++){
        wait(NULL);
    }
}

void cerrar_pipes(int fds[][2]){
    for(int i = 0; i < 3; i++){
        close(fds[i][READ]);
        close(fds[i][WRITE]);
    }
}

// Con numeros chicos funciona mas o menos. Habria que retocar un poco el tema de los rangos.
int main(){
    int num = 0;
    int socket_servidor = armar_socket_servidor();
    while(1){
        int socket_cliente = armar_socket_cliente(socket_servidor);
        recv(socket_cliente, &num, sizeof(num), 0);
        sleep(15);
        printf("Recibi el numero: %d, calculando...\n", num);
        int raiz_num = (int)sqrt(num);
        int rango = raiz_num/3;
        int desde = 2;
        int fd_h_p[3][2];
        for(int i = 0; i < 3; i++){
            pipe(fd_h_p[i]);
            pid_t pid_hijo = fork();
            if(pid_hijo == 0){
                // Soy el hijo
                proceso_hijo(fd_h_p[i], num, desde, desde + rango );
            }
            else{
                // Soy el padre
                close(fd_h_p[i][WRITE]);
                desde += rango;
            }
        }
        int es_primo = !leer_pipes(fd_h_p);
        // Cierro los pipes porque despues voy a crear otros para el proximo cliente
        cerrar_pipes(fd_h_p);
        wait_hijos();
        printf("Enviando respuesta: %d\n", es_primo);
        send(socket_cliente, &es_primo, sizeof(es_primo), 0);
        close(socket_cliente);
    }
    exit(EXIT_SUCCESS);
}
