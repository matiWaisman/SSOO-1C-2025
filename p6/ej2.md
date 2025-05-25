### Punto A
Si tenemos un disco de `128 Gb`, que equivale a $2^{37}$ `bytes` y bloques de `8 Kb` que equivalen a $2^{13}$ `bytes`, y cada entrada de la tabla es de `24 bytes`: 

Vamos a tener $\frac{2^{37}}{2^{13}} = 2^{24}$ entradas en la `FAT`. Y si cada entrada ocupa `24 bytes`, la `FAT` va a ocupar $2^{24} \times 24$ `bytes` en memoria. Esto equivale a aproximadamente `384 Mb`. 

### Punto B
`10 Mb` equivale a `10240 Kb`. Por lo que, para almacenar un archivo de ese tamaño, necesitamos primero dividir el tamaño del archivo por el tamaño de bloque para saber cuántos bloques ocupa un archivo. 

$\frac{10240}{8} = 1280$. Por lo tanto, necesitamos 1280 bloques para almacenar un archivo de este tamaño y nuestro disco tiene $2^{24}$ bloques disponibles para almacenar archivos. 

Por lo tanto, vamos a poder almacenar $\left\lfloor \frac{2^{24}}{1280} \right\rfloor = 13110$ archivos de ese tamaño en nuestro disco.

### Punto C
El archivo usa 7 bloques, aunque quizás el último bloque no lo use en su totalidad. El archivo pesa como mínimo `48 Kb`; si usa el último bloque en su totalidad, el archivo pesa `56 Kb`. Entonces, el archivo pesa entre 48 y 56 `Kb`.
