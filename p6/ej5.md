Asumo en todos los casos que ya tengo el `inodo` y la `FAT` en memoria.

### Punto A
Queremos leer los bloques: `1, 2, 3, 5, 7, 11, 13, 17, 23`.

- Con `FAT`: Como solo quiero acceder a ese bloque no necesito saber cual es el siguiente. Asi que va a ser un acceso a disco por bloque, lo que en total suma 9 acesos a disco. 
- Con `ext2`: Como tengo 12 entradas directas con estas puedo acceder del bloque 1 al 12 sin tener que leer un bloque de indireccion. Pero como se quiere tambien acceder al bloque numero 13,17 y 23 tambien va a haber que leer el bloque de indireccion simple. Por lo que hay que leer 9 bloques de datos + 1 de indireccion. 

### Punto B
Queremos leer los bloques: `1, 2, 3, 4, 5, 6, 10001`

- Con `FAT`: Mismo caso que el anterior. Va a haber que leer 7 bloques de datos. 
- Con `ext2`: Los primeros seis los vamos a leer de las entradas directas. Pero el bloque numero 10001

### Punto C
Queremos leer los bloques `13, 10000, 1000000`.

- Con `FAT`: Se tienen que leer 3 bloques de datos. 
- Con `ext2`:  

### Punto D
Queremos leer los bloques `14, 15...50`. 

- Con `FAT`: Se tienen que leer 37 bloques de datos. 
- Con `ext2`:

