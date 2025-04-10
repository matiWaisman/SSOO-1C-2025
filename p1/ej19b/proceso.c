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

int armar_socket_servidor(int numero_servidor){
    int socket_servidor; 

    struct sockaddr_un addr_servidor;
    
    addr_servidor.sun_family = AF_UNIX;
    sprintf(addr_servidor.sun_path, "/tmp/unix_socket_%d", numero_servidor);
    unlink(addr_servidor.sun_path);

    socket_servidor = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(socket_servidor, (struct sockaddr *) &addr_servidor, (unsigned int)sizeof(addr_servidor));
    listen(socket_servidor, 1);

    printf("Servidor conectado esperando conexiones\n");
    return socket_servidor;
}

int armar_socket_conexion(int numero_servidor_a_conectarte){
    int socket_conexion;
    struct sockaddr_un addr_servidor; // sockaddr_un especifica que es un adress de un socket de unix
    
    addr_servidor.sun_family = AF_UNIX;
    sprintf(addr_servidor.sun_path, "/tmp/unix_socket_%d", numero_servidor_a_conectarte);

    socket_conexion = socket(AF_UNIX, SOCK_STREAM, 0);

    int codigo_conexion = connect(socket_conexion, (struct sockaddr *)&addr_servidor, sizeof(addr_servidor));

    if(codigo_conexion == -1){
        perror("Hubo un problema al conectarse al servidor\n");
        exit(EXIT_FAILURE);
    }
    return socket_conexion;
}


// Cada proceso se va a conectar al que le manda el mensaje. Y recibe de otro proceso que se conecto a el
// Primer argumento que numero de proceso sos. Como son tres procesos no hace falta recibir como segundo a cual te conectas
int main(int argc, char* argv[]){
    int numero_proceso = atoi(argv[1]);
    int numero_proceso_a_conectarte = (numero_proceso % 3) + 1;

    int numero_a_enviar = numero_proceso - 1;
    int numero_recibido;
    int proximo_numero_a_enviar = numero_a_enviar + 3;

    int socket_servidor = armar_socket_servidor(numero_proceso);
    sleep(2);
    int socket_conexion = armar_socket_conexion(numero_proceso_a_conectarte);
    int socket_cliente = armar_socket_cliente(socket_servidor);

    while(proximo_numero_a_enviar <= 53){
        // Si estamos en el proceso 1 el es el primero en mandar un mensaje y luego recibe. En cambio los demas primero reciben y despues envian
        if(numero_proceso == 1){
            printf("Proceso %d le envia el numero %d a Proceso %d\n", numero_proceso, numero_a_enviar, numero_proceso_a_conectarte);
            write(socket_conexion, &numero_a_enviar, sizeof(numero_a_enviar));
            recv(socket_cliente, &numero_recibido, sizeof(numero_recibido), 0);
            numero_a_enviar = numero_recibido + 1;
        }
        else{
            recv(socket_cliente, &numero_recibido, sizeof(numero_recibido), 0);
            numero_a_enviar = numero_recibido + 1;

            printf("Proceso %d le envia el numero %d a Proceso %d\n", numero_proceso, numero_a_enviar, numero_proceso_a_conectarte);
            write(socket_conexion, &numero_a_enviar, sizeof(numero_a_enviar));
        }
        proximo_numero_a_enviar += 3;
    }
    close(socket_conexion);
    exit(EXIT_SUCCESS);
}