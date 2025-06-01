Asumo que la cantidad de punteros por bloque de indirección es la misma que la del ejercicio tres.

En cada tabla de indirección apunto a 512 bloques de datos. 

### Punto A
Queremos leer los bloques: `1, 2, 3, 5, 7, 11, 13, 17, 23`.

- Con `FAT`: Como solo quiero acceder a ese bloque, no necesito saber cuál es el siguiente. Así que va a ser un acceso a disco por bloque, lo que en total suma 9 accesos a disco.  
- Con `ext2`: Como tengo 12 entradas directas, con estas puedo acceder del bloque 1 al 12 sin tener que leer un bloque de indirección. Pero como se quiere también acceder a los bloques número 13, 17 y 23, también va a haber que leer el bloque de indirección simple. Por lo que hay que leer 9 bloques de datos + 1 de indirección.  

### Punto B
Queremos leer los bloques: `1, 2, 3, 4, 5, 6, 10001`

- Con `FAT`: Mismo caso que el anterior. Va a haber que leer 7 bloques de datos.  
- Con `ext2`: Los primeros seis los vamos a leer de las entradas directas. Pero el bloque número 10001 va a estar dentro de un bloque de la indirección doble. Así que voy a leer 7 bloques de datos, uno de la indirecta doble y otro al cual apunta la indirecta doble. Por lo que en total hay que leer 9 bloques.

### Punto C
Queremos leer los bloques `13, 10000, 1000000`.

- Con `FAT`: Se tienen que leer 3 bloques de datos.  
- Con `ext2`: El bloque número 13 lo podemos leer de la indirección simple. El número 10000 lo podemos leer de la indirección doble y el 1000000 de la triple. Por lo que hay que cargar dos bloques para leer el bloque número 13, tres para leer el 10000 y cuatro para leer el 1000000. Por lo que en total hay que leer 9 bloques.

### Punto D
Queremos leer los bloques `14, 15...50`.  

- Con `FAT`: Se tienen que leer 37 bloques de datos.  
- Con `ext2`: Los bloques del 14 al 50 están en la indirección simple. Por lo que hay que cargar el bloque de la indirección simple y después leer. Por lo que en total se necesitan leer 38 bloques. 
