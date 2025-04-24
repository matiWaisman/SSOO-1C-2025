El código sí cumple con que: "cada vez que un proceso lee de la variable compartida, previamente solicita el mutex y luego lo libera".  
Pero entendiendo que lo que queremos es que en todo momento todos los procesos estén operando sobre un mismo valor de `x`, el código proporcionado **no lo cumple** y sufre de **race condition**, porque varía la ejecución del programa dependiendo del orden de ejecución de los procesos, cosa que no debería pasar si estuviera bien programado.

El problema es que al actualizar el valor de `x`, el scheduler puede interrumpir al proceso antes de hacer el store en `x`, y otro proceso puede leer el valor viejo de `x` y hacer operaciones sobre él. Cuando el proceso que había sido pausado está a punto de hacer store, termina sobrescribiendo `x` con un valor viejo.  
Por lo tanto, no estamos cumpliendo que todos los procesos estén operando con el mismo valor de `x` en todo momento.

Por ejemplo:

- Proceso A: accede al lock y le da a la variable `y` el valor 0 (en varias instrucciones).
- Proceso A: entra a la rama true del if (en varias instrucciones).
- Proceso A: carga en un registro el valor de `y` (0).
- **Proceso A es interrumpido.**
- Proceso B: accede al lock y le da a la variable `y` el valor 0 (en varias instrucciones).
- Proceso B: guarda en `x` un 1 (en varias instrucciones).
- **Proceso B es interrumpido.**
- Proceso C: accede al lock y le da a la variable `y` el valor 1 (en varias instrucciones).
- Proceso C: guarda en `x` un 2 (en varias instrucciones).
- **Proceso C es interrumpido.**
- Proceso D: accede al lock y le da a la variable `y` el valor 2 (en varias instrucciones).
- Proceso D: guarda en `x` un 3 (en varias instrucciones).
- **Proceso D es interrumpido.**
- Proceso A: guarda en `x` un 1.

También podría pasar al revés: que los procesos estén disminuyendo el valor de `x` a 4 y se le vuelva a dar la ejecución a uno que guarda un 5 en `x`.

Para solucionar este comportamiento, habría que hacer que la asignación de `x` también esté dentro de la sección crítica, o usar variables atómicas.
