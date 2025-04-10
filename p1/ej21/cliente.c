#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<signal.h>

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
    pid_t pid_hijo = fork();
    if(pid_hijo != 0){
        // Estamos en el padre
        while(1){
            char buffer[1024];
            int n = read(socket_conexion, &buffer, sizeof(buffer));
            if(n < 0){
                kill(pid_hijo, SIGTERM);
                break;
            }
                tf("%s\n", buffer);
        }
    }
    else{
        // Estamos en el hijo
        char buffer[1024];
        while(1){
            printf("Contate algo: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // eliminás el '\n' si querés
                buffer[strcspn(buffer, "\n")] = '\0';
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