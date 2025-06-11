Dado el codigo: 

```c
void saludo(void) {
    char nombre[80];
    printf("Ingrese su nombre: ");
    gets(nombre);
    printf("Hola %s!\n", nombre);
}
```
## Punto A

El problema de seguridad está en la manera en la cual se le pide el input al usuario, con el `gets(nombre)`.

`gets` lee el input de `stdin` hasta encontrarse con un `\n` o un `EOF`. 

Por lo tanto se puede ingresar un string de más de 80 caracteres que va a sobreescribir otras variables o directamente cosas del stack. 

## Punto B

Recordando cómo se veía la memoria de una función: 

Direcciones de memoria (crece hacia arriba)
↑   Dirección más alta 
│   [ Parámetros de la función ]
│   [ Dirección de retorno ]
│   [ ebp ]
│   [ buf[0-7] ]  
│         .          
│         .
│   [ buf[1017-1024] ]
│   [ i ]
│   [ v ]
↓   Dirección más baja

```text
+-----------------------------+  
| Parámetros de la función   |  
+-----------------------------+
| Dirección de retorno       |   
+-----------------------------+
| EBP                        |   
+-----------------------------+
| Variables locales          |  
+-----------------------------+ 
```
Entonces si escribimos un string más grande que 80 caracteres vamos a causar un *buffer overflow* y vamos a poder pisar la memoria del stack. Dentro de la memoria del stack podemos pisar desde otras variables locales declaradas previamente, la dirección de retorno de la función, lo más grave, y parámetros de la función.

## Punto C  
Como tanto `gets` como `printf` reciben un puntero al búfer, si se escribe por fuera del búfer no se va a afectar el stack frame de esas funciones, porque estas solo acceden al stack frame de `saludo` mediante ese puntero.

Estas funciones no van a producir un overflow de su propio stack frame porque no tienen reservados 80 bytes estáticos para trabajar; simplemente acceden a memoria externa que les fue pasada como parámetro.

El único que se va a ver afectado es el stack frame de `saludo`, en el cual, en el espacio de las variables locales, están reservados los bytes necesarios para almacenar los 80 caracteres. Si se escribe más allá de esos 80 bytes, se van a pisar otros datos en el stack de `saludo`, como la dirección de retorno.

## Punto D  
No se soluciona el problema porque cuando ejecutamos la línea del `printf` el desbordamiento ya ocurrió cuando hicimos el `gets`.

La solución es reemplazar el `gets` que no puede recibir como parámetro el máximo a leer y usar una alternativa como `fgets` que sí permite setear una máxima cantidad de caracteres a leer.
