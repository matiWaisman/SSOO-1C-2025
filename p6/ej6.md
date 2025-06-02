### Punto A

En un sistema basado en `FAT`, suponiendo que la tabla ya está cargada en memoria, para llegar a los bloques de datos del archivo `/home/aprobar.txt` hay que hacer lo siguiente:

A partir del directorio raíz (del cual conocemos su ubicación), accedemos a su primer bloque de datos y comenzamos una búsqueda lineal por sus bloques para encontrar la *directory entry* correspondiente a `home`.

Si `home` no se encuentra en el primer bloque, hay que seguir leyendo los siguientes bloques del directorio raíz (siguiendo su cadena en la FAT) hasta encontrarlo. Esta búsqueda está acotada por el tamaño del directorio raíz.  
Supongamos que esto implica leer `r` bloques de disco.

Una vez encontrada la entrada del directorio `home`, obtenemos el número de su primer bloque de datos.  
Luego, realizamos una búsqueda lineal por los bloques del directorio `home` para encontrar la entrada correspondiente a `aprobar.txt`.  
Si no se encuentra en el primer bloque, hay que continuar leyendo los bloques de ese directorio (siguiendo su cadena en la FAT) hasta hallarlo.  
Supongamos que esto implica leer `h` bloques de disco.

Por lo tanto, para llegar al primer bloque de datos del archivo `aprobar.txt`, es necesario leer `r + h` bloques de disco correspondientes a los directorios. 
Luego de eso, pueden comenzarse los accesos a los bloques de datos del archivo en sí.

La búsqueda lineal en `FAT` es más fácil de implementar porque las *directory entries* tienen tamaño fijo. En cambio, en `ext2` o en sistemas con inodos, las entradas pueden tener nombres de longitud variable, lo que dificulta la implementación del recorrido.


### Punto B

En un sistema basado en `ext2`, si `/pepe.txt` es un enlace simbólico (*symbolic link*) a `/home/aprobar.txt`, el proceso sería el siguiente:

Primero, se busca dentro del inodo de `/` (ya cargado en memoria) la entrada `pepe.txt`.  
Esto implica recorrer los bloques de datos del directorio raíz leyendo sus entradas. Supongamos que se necesitan `r` lecturas de bloques de disco para encontrar la entrada.

Una vez encontrada la entrada `pepe.txt`, se accede al inodo correspondiente.  
Dado que se trata de un enlace simbólico, su contenido no apunta a los bloques de datos del archivo real, sino que contiene un path absoluto (`/home/aprobar.txt`) dentro de su primer bloque de datos. 

Luego, al parsear el path `/home/aprobar.txt`, es necesario nuevamente buscar en el directorio raíz (`/`) la entrada `home`. Supongamos que eso toma `r'` bloques de lectura.  
Finalmente, se accede al directorio `home` y se leen `h` bloques hasta encontrar `aprobar.txt`.

En total, se deben leer `r + 1 + r' + h` bloques de disco.

Si en lugar de un *symbolic link* fuera un *hard link*, bastaría con realizar la primera búsqueda para encontrar `pepe.txt`, ya que la entrada apunta directamente al mismo inodo del archivo real. Por lo tanto, no habría que seguir el path ni buscar `/home/aprobar.txt`.
