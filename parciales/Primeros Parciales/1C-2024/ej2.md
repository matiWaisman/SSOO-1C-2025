Tratando de traducir un poco la funcion recursiva, dice que: La prioridad del proceso i en el tiempo t va a ser: 
- Prioridad actual + $\alpha$ * Prioridad actual si el proceso esta en espera. 
- Prioridad actual + $\beta$ * Prioridad actual si el proceso esta ejecutandose.

## Punto A 
Si $\beta$ es mas grande que $\alpha$ le vamos a dar mas prioridad a los procesos que se estan ejecutando versus los que estan en espera listos para ejecutar. Por ejemplo en `First Come First Served` hasta que termine el proceso actual que tiene $\beta$ = 1 todos los demas procesos tienen prioridad cero. 