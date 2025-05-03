Para que cada proceso ejecute primero el conjunto de sentencias $a_i$ y, solo después de que todos los procesos hayan terminado de ejecutar sus respectivas $a_i$, comiencen a ejecutar las $b_i$, se puede utilizar el **método de la barrera**, como en el ejercicio 5.

La idea es usar dos semáforos:

- Un semáforo `mutex` para proteger una variable compartida `contador`, la cual se incrementará cada vez que un proceso termine su sección \( a_i \). Esto evita race conditions al modificar `contador`.
- Un semáforo `barrera` que servirá para bloquear a los procesos hasta que todos hayan llegado a ejecutar \( a_i \).

Cada proceso, al terminar de ejecutar sus $a_i$, entra en una **sección crítica** protegida por `mutex`, donde incrementa la variable `contador`. Al salir de la sección crítica si `contador` todavía no es igual a `N`, el proceso va a hacer un `barrera.wait()`.

El último proceso en llegar, es decir, en el cual `contador == N`, hace un `barrera.signal()` para despertar a uno de los procesos que estaban esperando.

Luego, cada proceso que se despierta del `barrera.wait()` realiza a su vez un `barrera.signal()` antes de continuar con sus $b_i$. Esto genera un efecto en cascada, donde se desbloquea a todos los procesos que estaban esperando en la barrera, asegurando que las $b_i$ comiencen solo después de que todos hayan terminado sus $a_i$.

Otra posibilidad puede ser, como en el ejercicio 6, usar una variable atómica `cantidadDeEjecutados`, y hasta que esta no sea igual a la cantidad de procesos, no se van a ejecutar los $b_i$. Tiene las mismas ventajas y desventajas, respecto a usar semáforos, que las nombradas en el ejercicio 6.

