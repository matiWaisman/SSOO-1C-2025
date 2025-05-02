#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include<sys/wait.h>

// Seguro que se puede hacer mas facil, despues de prueba y error fue lo que me salio mas facil. 

struct mensaje_interno {
   int lugar_cliente;
   int length;
   char buffer[1024];
   char nombre_cliente[100];
};

// Lo empaqueto para no correr riesgo de race condition
struct mensaje_a_enviar {
   char buffer[1024];
   char nombre_cliente[100];
};

enum { READ, WRITE }; 

int fd_hijos_padre[2]; // Por este pipe los hijos le van a comunicar al padre cuando un cliente manda un mensaje.
int fd_padre_hijos[5][2]; // Por este pipe el padre le va a comunicar a los hijos cuando un cliente manda un mensaje.
// Estos dos arrays de pipes trabajan en conjunto. Cuando un cliente manda un mensaje lo va a leer el hijo, 
// El hijo escribe en el pipe y le manda una señal al padre para que el padre le comunique a los demas hijos que llego un mensaje.  
// Los otros hijos ahi le van a mandar al cliente que otro proceso mando un mensaje.
pid_t pids_hijos[5];
int lugar_del_hijo;
int socket_cliente;

int armar_socket_cliente(int socket_servidor){
    struct sockaddr_un addr_cliente;

    unsigned int length_cliente = sizeof(addr_cliente);

    int socket = accept(socket_servidor, (struct sockaddr *) &addr_cliente, &length_cliente);

    return socket;
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

void handler_sigusr_1(){
    printf("Estamos en el padre resolviendo la lectura del mensaje\n");
    struct mensaje_interno mensaje_que_mandaron;
    read(fd_hijos_padre[READ], &mensaje_que_mandaron, sizeof(mensaje_que_mandaron));
    printf("Pude leer el mensaje desde el padre\n");
    printf("El mensaje que mandaron es: %s\n", mensaje_que_mandaron.buffer);
    struct mensaje_a_enviar msj;
    strcpy(msj.buffer, mensaje_que_mandaron.buffer);
    strcpy(msj.nombre_cliente, mensaje_que_mandaron.nombre_cliente);
    // Ahora escribo en el pipe de todos los hijos y les hago una señal para que lean su pipe
    for(int j = 0; j < 5; j++){
        if(pids_hijos[j] != 0 && mensaje_que_mandaron.lugar_cliente != j){
            write(fd_padre_hijos[j][WRITE], &msj, sizeof(msj));
            kill(pids_hijos[j], SIGUSR2);
        }
    }
}

void handler_sigusr_2(){
    printf("Soy un hijo y recibi que le tengo que avisar a mi cliente que le mandaron un mensaje\n");
    struct mensaje_a_enviar mensaje_recibido; 
    read(fd_padre_hijos[lugar_del_hijo][READ], &mensaje_recibido, sizeof(mensaje_recibido));
    printf("Recibi el mensaje: %s\n", mensaje_recibido.buffer);
    printf("Del usuario: %s\n", mensaje_recibido.nombre_cliente);
    send(socket_cliente, &mensaje_recibido, sizeof(mensaje_recibido), 0);
}

void handler_sigchld(){
    printf("Llegamos al sigchld\n");
    wait(NULL); // Lo waiteamos para que no quede zombie
    printf("Salimos del wait \n");
    int lugar_del_hijo_muerto;
    read(fd_hijos_padre[READ], &lugar_del_hijo_muerto, sizeof(lugar_del_hijo_muerto));
    printf("Ya waiteamos al cliente numero %d\n", lugar_del_hijo_muerto);
    pids_hijos[lugar_del_hijo_muerto] = 0;
    pipe(fd_padre_hijos[lugar_del_hijo_muerto]); // Restauramos el pipe que habiamos cerrado
}

// Usar un solo pipe en donde los hijos le comuniquen el mensaje y quien son.
// Cuando el hijo recibe un mensaje le sigue mandando el signal
int main(){
    int socket_servidor = armar_socket_servidor();
    // Inicializamos los pipes de comunicacion.  
    for(int i = 0; i < 5; i++){
        pids_hijos[i] = 0; 
        pipe(fd_padre_hijos[i]);
    }
    pipe(fd_hijos_padre);
    // Si pids_hijos[i] == 0 es porque no hay un cliente 
    signal(SIGUSR1, handler_sigusr_1);
    signal(SIGCHLD, handler_sigchld);
    while(1){
        socket_cliente = armar_socket_cliente(socket_servidor);
        char nombre_cliente[100];
        recv(socket_cliente, &nombre_cliente, sizeof(nombre_cliente), 0);
        printf("El nombre del cliente que se conecto es: %s\n", nombre_cliente);
        printf("Se conecto un cliente\n");
        // Hasta que no haya lugar para un nuevo cliente nos quedamos colgados
        lugar_del_hijo = -1;
        while(lugar_del_hijo == -1){
            for(int i = 0; i < 5; i++){
                if(pids_hijos[i] == 0){
                    lugar_del_hijo = i;
                    break;
                }
            }
        }
        pid_t pid_hijo = fork();
        if(pid_hijo != 0){
            // Soy el padre
            pids_hijos[lugar_del_hijo] = pid_hijo;
            close(fd_padre_hijos[lugar_del_hijo][READ]); // Capaz hay que hacer que este no se cierre nunca para cuando haya que reiniciar las conexiones
        }
        else{
            // Soy el hijo
            signal(SIGUSR2, handler_sigusr_2);
            pid_t mi_pid = getpid(); // Capaz esta de mas esto 
            close(fd_hijos_padre[READ]);
            close(fd_padre_hijos[lugar_del_hijo][WRITE]); // Capaz este no hay que cerrarlo para cuando haya que reiniciar las conexiones
            while(1){ // (Mientras haya una conexion)
                struct mensaje_interno mensajito;
                mensajito.lugar_cliente = lugar_del_hijo;
                strcpy(mensajito.nombre_cliente, nombre_cliente); // Guardo en mensajito.nombre_cliente el nombre del buffer
                char buffer[1024];
                mensajito.length = recv(socket_cliente, &mensajito.buffer, sizeof(buffer), 0);
                printf("Recibi un mensaje!\n");
                printf("%s\n", buffer);
                if(mensajito.length < 1){
                    break;
                }
                // Le mandamos al padre nuestro lugar en la lista y un puntero del mensaje que me mandaron
                printf("Mandaron el mensaje: %s\n", mensajito.buffer);
                write(fd_hijos_padre[WRITE], &mensajito, sizeof(struct mensaje_interno));
                kill(getppid(), SIGUSR1); // Le avisamos al padre que lea el pipe y le avise a los demas
            }
            printf("Se desconecto el cliente numero: %d\n", lugar_del_hijo);
            // Le escribo al padre que numero de cliente soy asi reiniciamos el pipe de ese lugar 
            write(fd_hijos_padre[WRITE], &lugar_del_hijo, sizeof(lugar_del_hijo));
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}
