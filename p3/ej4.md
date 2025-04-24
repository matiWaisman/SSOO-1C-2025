Suponiendo que el funcionamiento de `wait` y `signal` en pseudocódigo es: 

```
wait(s):
    while (s <= 0):
        dormir();
    s--;

signal(s):
    s++;
    if(alguien espera por s):
        despertar a alguno;
```

Si no se ejecuta atómicamente, puede ocurrir un escenario como: 

- Un proceso P1 ejecuta `signal(s)`. Incrementa `s` (ahora `s = 1`) pero es interrumpido antes de ejecutar el `despertar a alguno`.
- El scheduler le da CPU a otro proceso P2 que está en `wait(s)`. Como `s > 0`, sale del bucle y ejecuta `s--`, entrando así a la sección crítica.
- Luego, P1 retoma su ejecución y ejecuta el `despertar a alguno`. Esto hace que otro proceso en espera (P3) también entre a la sección crítica.

Haciendo que haya dos procesos accediendo al mismo recurso al mismo tiempo, violando la propiedad de exclusión mutua.

Otro problema surgiría si dos procesos ejecutan parte del `wait`: ambos cargarían en un registro que `s` vale 1 y no se enterarían de que el otro proceso disminuyó el valor de `s` a 0. Así los dos procesos accederían al recurso a la vez.
