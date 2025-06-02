### Punto A
Si cada bloque ocupa `2 Kb`, los identificadores de bloques miden `24 bits` y el hash mide `16 bits`: 

Vamos a poder dividir la memoria en: $\frac{16 Gb}{2 Kb} = 2^{23}$ bloques de `2 Kb`. Por lo que si cada identificador mide `24 bits` la tabla va a ocupar: $2^{23} \times 24 \text{ bits} = 24 \text{ Mb}$. 

El tamaño de la tabla de archivos (la tabla de hash) va a ser igual a la cantidad de bloques por el tamaño del hash. Esto es: $2^{23} \times 16 \text{ bits} = 16 \text{ Mb}$. 

Por lo que, suponiendo que tenemos la tabla de la `FAT` duplicada en disco y una sola tabla de hash, nos va a sobrar $16 \text{ Gb} - 24 \times 2 \text{ Mb} - 16 \text { Mb} = 15 \text{ Gb} + 857.6 { Mb}$.

### Punto B
Si en promedio los archivos pesan `1 Kb`, convendría que los bloques sean igual al tamaño de sector para que la gran mayoría de los archivos entren en una entrada de la `FAT` y no tener fragmentación interna, porque si los bloques fueran de `2 Kb`, la gran mayoría de los archivos va a estar desperdiciando memoria y va a dejar parte de sus bloques asignados sin usar.

Pero como la unidad mínima para el bloque es de dos sectores, conviene que cada bloque mida dos sectores. Porque si elegimos 4 o 8, pasaría que cada archivo está desperdiciando o la mitad del bloque o tres cuartos del bloque. 

Como 2 sectores es una unidad chica de almacenamiento, va a pasar que los archivos de mayor tamaño no se van a ver perjudicados porque los bloques son chicos, por lo que un archivo que mida más de dos bloques va a tener que usar más de un bloque para el archivo y va a dejar poca fragmentación interna en el último, a diferencia de si los bloques fueran más grandes. 

Pero sí se va a ver perjudicado el tamaño de la `FAT` y la `Hash Table`, que van a ocupar más espacio en memoria. 

Con bloques de `2 Kb` podemos dividir la memoria en $\frac{16 Gb}{2 Kb} = 2^{23}$ bloques. Por lo que conviene que las direcciones sean de 24 bits para poder representar a cada uno de los bloques de la `FAT`. 

### Punto C
Si el tamaño promedio de los archivos es de `16 MB`, para no perjudicar el tamaño de las estructuras del `File System` en memoria conviene que los bloques sean de 8 sectores, para que el archivo promedio ocupe menos bloques. 

Con bloques de `8 Kb` podemos dividir el disco en $\frac{16 Gb}{8 Kb} = 2^{21}$ bloques. Para poder direccionar estos bloques, podemos usar direcciones de `24 bits`. 
