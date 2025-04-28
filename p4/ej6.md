### Punto A
Como el acceso a las páginas es secuencial y no se repite el acceso a una misma página hasta la siguiente iteración del ciclo, todo el tiempo se van a estar produciendo page faults y reemplazos de páginas.
Como los accesos son secuenciales y no se reutilizan páginas a corto plazo, el orden de reemplazo va a ser el mismo para los tres algoritmos.

En esta situación ninguno de los tres algoritmos es efectivo para manejar esta dinámica con una cantidad de frames menor que la longitud de la secuencia, porque en los tres, cuando lleguemos a requerir un elemento, este ya va a estar descartado desde hace rato porque fue swapeado.

### Punto B
Si tenemos 500 frames, lo que podemos hacer es tener un único frame en el cual vamos haciendo los reemplazos, siempre cambiando ese elemento, y en los 499 frames restantes tener siempre cargados los frames del 0 al 498 o del 511 al 12.

Entonces, el rango de elementos fijos va a dar page fault en la primera iteración del ciclo y después van a ser siempre hits. Los elementos que faltan van a ser misses, y el elemento random es altamente probable que ya lo tengamos cargado en memoria. Así que por ciclo tendríamos 499 hits asegurados, 12 misses asegurados, más el random, que es más probable que sea hit a que sea miss.
