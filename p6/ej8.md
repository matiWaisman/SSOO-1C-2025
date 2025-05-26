### Punto A
Si cada bloque ocupa `2 Kb`, los identificadores de bloques miden `24 bits` y el hash mide `16 bits`: 

Vamos a poder dividir la memoria en: $\frac{16 Gb}{2 Kb} = 2^{23}$ bloques de `2 Kb`. Por lo que si cada identificador mide `24 bits` la tabla va a ocupar: $2^{23} \times 24 \text{ bits} = 24 \text{ Mb}$. 

El tamaño de la tabla de archivos (la tabla de hash) va a ser igual a la cantidad de bloques por el tamaño del hash. Esto es: $2^{23} \times 16 \text{ bits} = 16 \text{ Mb}$. 

Por lo que suponiendo que tenemos la tabla de la `FAT` duplicada en disco y una sola tabla de hash nos va a sobrar $16 \text{ Gb} - 24 \times 2 \text{ Mb} - 16 \text { Mb} = 15 \text{ Gb} + 857.6 { Mb}$

### Punto B
Si en promedio los archivos pesan `1 Kb` convendría que los bloques sean igual al tamaño de sector para que la gran mayoría de los archivos entren en una entrada de la `FAT` y no tener fragmentación interna, porque si los bloques fueran de `2 Kb` la gran mayoría de los archivos van a estar desperdiciando memoria y van a dejar parte de sus bloques asignados sin usar.

En cambio todos los archivos que ocupen más de `1 Kb` van a tener que usar más de un bloque. Como `1 Kb` es una unidad chica, si no el archivo no es un múltiplo exacto de `1 Kb` aun así la memoria desperdiciada en el bloque que deja vacío va a ser mucho menor que la memoria desperdiciada por los archivos que pesan `1 Kb` si los bloques fueran más grandes.

Con bloques de `1 Kb` podemos dividir la memoria en $\frac{16 Gb}{1 Kb} = 2^{24}$ bloques. 

Podemos usar un tamaño de hash de `24 bits` porque con este tamaño podemos representar todos los bloques de la `FAT`. 

La desventaja de esto es que la `FAT` al tener tantas entradas va a ser bastante grande a comparación de si los bloques tuvieran mayor tamaño. Pero si la cantidad de archivos que miden `1 Kb` es grande va a terminar amortizándose la memoria que no perdemos en fragmentación interna versus el tamaño de la tabla en memoria principal. 

### Punto C
