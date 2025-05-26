### Punto A
Para leer un archivo de `40 Kb` va a haber que, como mínimo, leer 10 bloques de datos. Como en `ext2` tenemos diez punteros directos, no va a hacer falta leer un bloque de indirección.

### Punto B
Para leer un archivo de `80 Kb` va a haber que, como mínimo, leer 20 bloques de datos. Como con los punteros directos solo podemos leer los primeros diez bloques, ahora vamos a tener que usar la indirección simple para poder leer los diez bloques que faltan.  
Entonces, también vamos a tener que leer el bloque de la indirección simple.

Por lo que, en total, vamos a tener que leer 21 bloques.
