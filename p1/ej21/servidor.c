#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include<sys/wait.h>

enum { READ, WRITE }; 

int fd_hijos_padre[5][2]; // Por este pipe los hijos le van a comunicar al padre cuando un cliente manda un mensaje.
int fd_padre_hijos[5][2]; // Por este pipe el padre le va a comunicar a los hijos cuando un cliente manda un mensaje.
// Estos dos arrays de pipes trabajan en conjunto. Cuando un cliente manda un mensaje lo va a leer el hijo, 
// el hijo escribe en el pipe y le manda una señal al padre para que el padre le comunique a los demas hijos que llego un mensaje.  
// Los otros hijos ahi le van a mandar al cliente que otro proceso mando un mensaje.
pid_t pids_hijos[5];
int lugar_del_hijo;
int socket_cliente;

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

void handler_sigchld(){
    printf("Estamos en el padre resolviendo la lectura del mensaje\n");
    pid_t pid_que_llamo;
    int status;
    pid_que_llamo = wait(&status); // ¡ahora sí!
    printf("%d\n", pid_que_llamo);
    char buffer[1024];
    // Primero busco la posicion del elemento que me hizo el sigchild
    int i;
    for(i = 0; i < 5; i++){
        if(pids_hijos[i] == pid_que_llamo){
            break;
        }
    }
    // La posicion i es el que nos hizo el sigchild. Asi que leo su pipe
    printf("Tengo que leer el pipe %d\n", i);
    read(fd_hijos_padre[i][READ], &buffer, sizeof(buffer));
    printf("%s\n", buffer);
    // Ahora escribo en el pipe de todos los hijos y les hago una señal para que lean su pipe
    for(int j = 0; j < 5; j++){
        if(pids_hijos[j] != 0 && i != j){
            write(fd_padre_hijos[j][WRITE], &buffer, sizeof(buffer));
            kill(pids_hijos[j], SIGUSR1);
        }
    }
}

void handler_sigusr(){
    printf("Soy un hijo y recibi que le tengo que avisar a mi cliente que le mandaron un mensaje\n");
    char buffer[1024];
    read(fd_padre_hijos[lugar_del_hijo][READ], &buffer, sizeof(buffer));
    printf("El mensaje es: %s", buffer);
    send(socket_cliente, &buffer, sizeof(buffer), 0);
}

int main(){
    int socket_servidor = armar_socket_servidor();
    
    for(int i = 0; i < 5; i++){
        pids_hijos[i] = 0; 
        pipe(fd_hijos_padre[i]);
        pipe(fd_padre_hijos[i]);
    }
    // Si pids_hijos[i] == 0 es porque no hay un cliente 
    signal(SIGCHLD, handler_sigchld);
    while(1){
        armar_socket_cliente(socket_servidor);
        printf("Se conecto un cliente\n");
        for(int i = 0; i < 5; i++){
            if(pids_hijos[i] == 0){
                lugar_del_hijo = i;
                break;
            }
        }
        pid_t pid_hijo = fork();
        if(pid_hijo != 0){
            // Soy el padre
            pids_hijos[lugar_del_hijo] = pid_hijo;
            close(fd_padre_hijos[lugar_del_hijo][READ]);
            close(fd_hijos_padre[lugar_del_hijo][WRITE]);
        }
        else{
            // Soy el hijo
            signal(SIGUSR1, handler_sigusr);
            close(fd_hijos_padre[lugar_del_hijo][READ]);
            close(fd_padre_hijos[lugar_del_hijo][WRITE]);
            while(1){ // (Mientras haya una conexion)
                char buffer[1024];
                int n = recv(socket_cliente, &buffer, sizeof(buffer), 0);
                printf("Recibi un mensaje!\n");
                printf("%s\n", buffer);
                if(n < 1){
                    break;
                }
                write(fd_hijos_padre[lugar_del_hijo][WRITE], &buffer, sizeof(buffer));
                kill(getppid(), SIGCHLD);
            }
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}
