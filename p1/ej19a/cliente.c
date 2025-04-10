#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int armar_socket_conexion(){
    int socket_conexion;
    struct sockaddr_un addr_servidor; // sockaddr_un especifica que es un adress de un socket de unix
    
    addr_servidor.sun_family = AF_UNIX;
    strcpy(addr_servidor.sun_path, "/tmp/unix_socket");

    socket_conexion = socket(AF_UNIX, SOCK_STREAM, 0);

    int codigo_conexion = connect(socket_conexion, (struct sockaddr *)&addr_servidor, sizeof(addr_servidor));

    if(codigo_conexion == -1){
        perror("Hubo un problema al conectarse al servidor\n");
        exit(EXIT_FAILURE);
    }
    return socket_conexion;
}

int main(){
    int num = 0;
    int socket_conexion = armar_socket_conexion();
    while(num < 49){
        read(socket_conexion, &num, sizeof(int));
        printf("Proceso 2 leyo el numero %d del proceso 1\n", num);
        num = num + 1;
        printf("Proceso 2 le envia el numero %d a Proceso 1\n", num);
        write(socket_conexion, &num, sizeof(int));
    }

    close(socket_conexion);
    exit(EXIT_SUCCESS);
}