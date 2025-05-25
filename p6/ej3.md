### Punto A 

Si los bloques son de tamaño `4 Kb`, y el direccionamiento a bloques de disco es de `8 Bytes`: 

Con cada entrada directa podemos direccionar un bloque de tamaño `4 Kb`, por lo que con estos cinco bloques podemos soportar archivos de hasta `20 Kb`. 

Cada "tabla indirecta" va a tener $\frac{\text{size bloque}}{\text{direccionamiento de bloque}}$ entradas. Que en este caso van a ser $\frac{\text{4 Kb}}{\text{8 bytes}} = 512$ entradas.

Por lo que si podemos apuntar a 512 bloques y cada bloque pesa `4 Kb` vamos a poder direccionar archivos de hasta `2 Mb` por cada indireccion simple. Como en este caso tenemos dos indirecciones simples nos suman `4 Mb` 

Y en la indireccion doble apuntamos a un bloque que contiene 512 punteros los cuales apuntan a otro bloque con 512 punteros a bloques de datos, por lo que podemos apuntar a $512 \times 512$ bloques de datos. Si cada bloque de datos pesa `4 Kb` podemos direccionar archivos de: $512 \times 512 \times \text{4 Kb} = \text{1 Gb}$ de tamaño. 

Por lo que nuestro sistema soporta archivos de hasta un maximo de `20 Kb + 4 Mb + 1 Gb`.

### Punto B


### Punto C
Para procesar un archivo de `5 Mb` necesitariamos solo en bloques de datos $\frac{\text{5 Mb}}{\text{4 Kb}} = 1280$ bloques de datos. 

Para los primeros cinco de los 1280 bloques de datos tenemos las cinco entradas directas. Asi que faltan direccionar 1275 bloques de datos. 

Cada bloque de indirección simple puede direccionar 512 bloques de datos. Asi que nos faltan direccionar 251 bloques de datos. 

Para los restantes hay que caer en la indireccion doble. Habria que cargar el bloque de indireccion doble y el primero al que apunta. 

Por lo que en total se necesitan los 1280 bloques de datos + 2 bloques de indirección simple + bloque de indireccion doble + primer bloque al que apunta el bloque de indireccion doble. Entonces necesitamos acceder a  1284 bloques para procesar un archivo completo de `5 Mb`

(Chequear si no hay que solamente contar los de datos pero no creo)