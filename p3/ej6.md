Antes de empezar a ejecutar los procesos en paralelo, definimos en memoria compartida: 

```c
atomic<int> count
count.set(0)
```

Ya dentro del código de los procesos: 

```c
preparado()
count.getAndInc()
while(count.getAndAdd(0) != n){}
critica()
```

Dejo como comparación el código de la solución usando semáforos: 

Primero definimos en memoria compartida:

```c
mutex = sem(1)
count = 0
mutex = sem(0)
```

Y el código de los procesos va a ser:

```c
preparado()
mutex.wait()
count = count + 1
mutex.signal()
if (count == n):
    barrera.signal()
    critica()
else:
    barrera.wait()
    barrera.signal()
    critica()
```

La primera solución genera un código un poco más legible. 

El problema de la primera solución es que genera *busy waiting* en el `while`, mientras que la segunda solución requiere de más soporte de parte del SO, que tiene que hacer *context switches* para el uso del semáforo. 

Si sabemos que los procesos van a tardar poco en hacer la función `preparado`, tiene más sentido usar la primera, porque los procesos estarían poco tiempo haciendo busy waiting y sería una mala idea perder tiempo de ejecución haciendo context switches con el semáforo. Pero si se tarda mucho en ejecutar `preparado`, se amortiguaría el tiempo de hacer el context switch y sería más caro el tiempo de CPU que perdemos haciendo busy waiting. 

Para las variables atómicas el soporte viene del lado del hardware, el cual implementa en Hardware las instrucciones para trabajar con variables atomicas.

Mientras que con los semáforos el soporte viene del lado del sistema operativo, que con el scheduler se encarga de encolar a los procesos que hacen `wait` y luego se encarga de despertar a alguno de los procesos encolados cuando alguien hace un `signal`.
