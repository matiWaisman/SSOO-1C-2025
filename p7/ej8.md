## Punto A
Un valor de la cadena `dir` que, además de listar el directorio actual, muestre el contenido del archivo `/etc/passwd` puede ser `/home | cat /etc/passwd`. Porque con el pipe hacemos que el output de `ls /home` sea el input de `cat /etc/passwd`, pero como en el comando del `cat` no usamos parámetros, no le va a dar bola a ese parámetro y va a ejecutarlo.

Otras posibilidades mejores o con más sentido serían usar `;`, que hace que se ejecute un comando después del otro, o `&&`, que hace que se ejecute un comando después del otro solo en caso de que el primero tenga éxito (cosa que va a pasar siempre si le pasamos bien la ruta). O si no, también `&`.

## Punto B
Si ahora el tercer parámetro de `snprintf` es `ls \" %s\` aun así podemos hacer `/home ; cat /etc/passwd`. Nos va a pedir que ingresemos un carácter más que está esperando por el último `\` y, después de eso, nos va a mostrar el `cat`.

## Punto C
Si no se puede usar `;`, se pueden usar pipes o el `&&` o `&`, como mencioné en el A. O tambien se puede cambiar el path para ejecutar a `ls` por un programa nuestro, un `enviroment variable attack`.

## Punto D
Para que no tenga problemas de seguridad, el `wrapper` habría que sanitizar el input y usar el path absoluto de ls.

Luego de hacer el `snprintf`, podemos chequear si `cmd` contiene `;`, `|`, `&` o `&&` y, si tiene, ni ejecutar el `system`, o modificar el código así: 

```c
#define BUF_SIZE 1024
void wrapper_ls(const char * dir) {
    char cmd[BUF_SIZE];
    snprintf(cmd, BUF_SIZE-1, "/bin/ls %s", dir);
    char *cmd_sanitized = strtok(cmd, " ;&|() ");
    system(cmd_sanitized);
}
