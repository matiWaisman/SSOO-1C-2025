### Punto A

En un sistema basado en `FAT` suponiendo que la tabla ya esta cargada en memoria, para llegar a los bloques de datos del archivo `/home/aprobar.txt` hay que:

Usando la entrada distinguida del directorio `root`, accedemos a su primer bloque de datos y comenzamos una búsqueda lineal por sus bloques para encontrar la entrada correspondiente a `home`.  
Si `home` no se encuentra en el primer bloque, hay que seguir leyendo los bloques de datos del directorio `root` (siguiendo su cadena en la FAT) hasta encontrarlo.  
Supongamos que esto implica leer `r` bloques de disco.

Una vez encontrada la entrada del directorio `home`, obtenemos el número de su primer bloque de datos.  
Luego, realizamos una búsqueda lineal dentro del directorio `home` para encontrar la entrada correspondiente a `aprobar.txt`.  
Si no se encuentra en el primer bloque, hay que continuar leyendo los bloques de ese directorio (siguiendo su cadena en la FAT) hasta hallarlo.  
Supongamos que esto implica leer `h` bloques de disco.

Por lo tanto, para llegar al primer bloque de datos del archivo `aprobar.txt`, es necesario leer `r + h` bloques de disco correspondientes a los directorios.  
Luego de eso, pueden comenzarse los accesos a los bloques de datos del archivo en sí.

### Punto B
En un sistema basado en `ext2` si `/pepe.txt` es un `symbolic link` a `/home/aprobar.txt`: 

Primero hay que buscar dentro del inodo de `root`, ya cargado en memoria, el archivo `pepe.txt`. Dependiendo de en que bloque de datos este va a cambiar cuantos bloques de disco vamos a tener que leer, si esta dentro de una de los bloques con direccion directa o indireccion. Supongamos que toma `r` lecturas de bloques del disco. 

Una vez encontrado el inodo de `/pepe.txt` hay que cargar el inodo al que apunta `/pepe.txt` que va a ser un inodo que esta dentro de `/home`. Asi que hay que sumar otra lectura a disco. 

Con el inodo al cual apunta 

