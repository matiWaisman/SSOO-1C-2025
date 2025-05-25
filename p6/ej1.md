Si el disco no tiene memoria caché y la `FAT` no queda almacenada en memoria, para acceder a la primera entrada:

- Leemos el índice donde se encuentra el primer bloque del archivo en la `FAT` (acceso a disco) y obtenemos dónde está el segundo bloque. También leemos a disco para efectivamente obtener el contenido del primer bloque del archivo.

<p align="center">.</p>
<p align="center">.</p>
<p align="center">.</p>

- Leemos el índice donde se encuentra el bloque `i` del archivo en la `FAT` (acceso a disco) y obtenemos dónde está el bloque `i+1`. También leemos a disco para efectivamente obtener el contenido del `i-ésimo` bloque del archivo.

<p align="center">.</p>
<p align="center">.</p>
<p align="center">.</p>

- Leemos el índice donde se encuentra el bloque `N` del archivo en la `FAT` (acceso a disco) y, como `N` es el último bloque, este va a tener un `EOF` que nos indica que este es el último bloque a leer del archivo. Por último, leemos a disco para obtener el contenido del bloque número `N` en disco.

Por lo tanto, hay `2N` accesos a disco.
