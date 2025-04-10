#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

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
    strcpy(addr_servidor.sun_path, "unix_socket");
    unlink(addr_servidor.sun_path);

    socket_servidor = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(socket_servidor, (struct sockaddr *) &addr_servidor, (unsigned int)sizeof(addr_servidor));
    listen(socket_servidor, 1);

    printf("Servidor conectado esperando conexiones\n");
    return socket_servidor;
}

int main(){
    int num = 0;
    int socket_servidor = armar_socket_servidor();
    int socket_cliente = armar_socket_cliente(socket_servidor);
    while(num <= 50){
        printf("Proceso 1 le envia el numero %d a Proceso 2\n", num);
        send(socket_cliente, &num, sizeof(num), 0);
        recv(socket_cliente, &num, sizeof(num), 0);
        printf("Proceso 1 leyo el numero %d del proceso 2\n", num);
        num = num + 1;
    }
    close(socket_cliente);
    exit(EXIT_SUCCESS);
}