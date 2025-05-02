#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/wait.h>

struct mensaje_a_recibir {
    char buffer[1024];
    char nombre_cliente[100];
 };

void handler_sigchld(){
    wait(NULL);
}

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

// Van a haber dos procesos. Uno que se encarga de leer mensajes y el otro que se encarga de mandarlos
// El padre se va a encargar de recibir los mensajes y mostrarlos por pantalla y el hijo se va a encargar de mandar los mensajes
int main(int argc, char* argv[]){
    char* nombre_usuario = argv[1];
    int socket_conexion = armar_socket_conexion();
    // Le mandamos al servidor nuestro nombre 
    send(socket_conexion, nombre_usuario, strlen(nombre_usuario) + 1, 0);
    signal(SIGCHLD, handler_sigchld);
    pid_t pid_hijo = fork();
    if(pid_hijo != 0){
        // Estamos en el padre. El padre recibe los mensajes. 
        while(1){
            struct mensaje_a_recibir mensaje_recibido;
            int n = read(socket_conexion, &mensaje_recibido, sizeof(mensaje_recibido));
            if(n < 0){
                kill(pid_hijo, SIGTERM);
                break;
            }
                // Control secuence para borrar el contate algo 
                printf("%s: %s\n", mensaje_recibido.nombre_cliente, mensaje_recibido.buffer);
                printf("? ");
                fflush(stdout); 
        }
    }
    else{
        // Estamos en el hijo
        char buffer[1024];
        while(1){
            printf("? ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0'; // Cambiar a ponerle un /0 en la pos 1023
                send(socket_conexion, buffer, strlen(buffer) + 1, 0);
            } else {
                break;
            }
        }
        exit(EXIT_SUCCESS);
    }
    close(socket_conexion);
    exit(EXIT_SUCCESS);
}