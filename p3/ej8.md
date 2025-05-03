## Caso 1

Definimos primero en memoria compartida:

```c
permisoA = sem(1)
permisoB = sem(0)
permisoC = sem(0)
```

El código para el proceso A va a ser:

```c
while(1){
    permisoA.wait();
    print("a");
    permisoB.signal();
}
```

El código para el proceso B va a ser:

```c
while(1){
    permisoB.wait();
    print("b");
    permisoC.signal();
}
```

Y el del proceso C va a ser:

```c
while(1){
    permisoC.wait();
    print("c");
    permisoA.signal();
}
```

No va a haber **starvation** porque se va a generar una cadena en la que, cuando se ejecuta un proceso, este destraba al otro de su `wait`. Es importante remarcar que el semáforo que controla la ejecución del proceso A tiene que empezar en uno, porque si no, va a haber un **deadlock**.

---

## Caso 2

Suponiendo que el proceso B se ejecuta en único Thread:

Definimos primero en memoria compartida:

```c
permisoA = sem(0)
permisoB = sem(1)
permisoC = sem(0)
```

El código para el proceso A va a ser:

```c
while(1){
    permisoA.wait();
    print("a");
    permisoB.signal();
}
```

El del proceso B va a ser:

```c
cantidadEjecucionesB = 0; // Obs que no hace falta que sea atomica.
while(1){
    permisoB.wait();
    cantidadEjecucionesB++;
    print("b");
    if(cantidadEjecucionesB == 1){
        permisoB.signal();
    }
    else{
        cantidadEjecucionesB = 0;
        permisoC.signal();
    }
}
```

Y el del proceso C va a ser:

```c
while(1){
    permisoC.wait();
    print("c");
    permisoA.signal();
}
```

No va a haber **starvation** porque se va a mantener la misma cadena que en el punto anterior. Es importante remarcar que, en este caso, el semáforo que tiene que empezar en 1 es el del proceso B para que se ejecute primero.

Para manejar que el proceso B se ejecute dos veces seguidas, usamos la variable `cantidadEjecucionesB`. Cuando es la primera ejecución, volvemos a hacer `signal` a nuestro permiso para poder ejecutarnos inmediatamente, porque el proceso C sigue esperando su semáforo. En la segunda ejecución de B, recién ahí prendemos el semáforo de C, y B no se volverá a ejecutar hasta que lo haga A y levante el semáforo de B.

---

## Caso 3

Primero definimos en memoria compartida:

```c
permisoConsumidor = sem(0)
permisoProductor = sem(1)
mutex = sem(1)
cantidadConsumidores = 0 // Como tanto el proceso B como el proceso C van a usar esta variable o la hacemos atomica o usamos un mutex. En este caso uso un mutex.
```

El código para el proceso A es:

```c
while(1){
    permisoProductor.wait();
    print("a");
    permisoConsumidor.signal();
    permisoConsumidor.signal();
}
```

El código para el proceso B:

```c
while(1){
    permisoConsumidor.wait();
    print("b");
    mutex.wait();
    cantidadConsumidores++;
    if(cantidadConsumidores == 2){
        cantidadConsumidores = 0;
        permisoProductor.signal();
    }
    mutex.signal();
}
```

Y para el proceso C:

```c
while(1){
    permisoConsumidor.wait();
    print("c");
    mutex.wait();
    cantidadConsumidores++;
    if(cantidadConsumidores == 2){
        cantidadConsumidores = 0;
        permisoProductor.signal();
    }
    mutex.signal();
}
```

El ciclo de ejecución comienza con el proceso `A` pasando su semáforo, ya que lo inicializamos en 1. Este va a incrementar en dos los semáforos de los consumidores y luego se volverá a bloquear hasta que su semáforo sea incrementado nuevamente por el último de los dos consumidores.

Los procesos `B` y `C` tienen exactamente el mismo código; lo único que cambia es la ejecución del `print`, que representa la ejecución de su programa ficticio.

Estos procesos esperan a que `A` produzca para poder pasar su semáforo. Una vez que lo hacen y ejecutan su función, acceden a una zona protegida para modificar la variable no atómica `cantidadConsumidores`. En esa zona, deciden si todavía queda un proceso por consumir o si ya es hora de volver a producir. Como ambos no pueden acceder simultáneamente a la zona crítica, el primero en entrar simplemente actualizará la variable, mientras que el segundo, luego de actualizarla, le enviará el `signal` al productor para que produzca nuevamente.

No va a haber inanición porque en cada ciclo de ejecución se realizan dos señales a los consumidores, y de esos dos, uno siempre le envía una señal al productor para que vuelva a ejecutarse.

Es importante observar que pueden darse casos como `ABB`, `ABC`, `ACB` o `ACC` debido a lo siguiente:

- Para que se ejecute dos veces el mismo consumidor, el *scheduler* puede darle suficiente tiempo como para que llegue a ejecutar el `while` dos veces, o puede interrumpirlo durante su segunda ejecución. Como `A` y el otro consumidor están bloqueados en un `wait`, el consumidor que estaba en ejecución puede continuar con su segundo ciclo.
- Para que se ejecuten dos consumidores distintos, el *scheduler* puede interrumpir a uno mientras está corriendo y darle tiempo de CPU al otro. Si el primero fue interrumpido mientras se encontraba en la zona protegida, el segundo no podrá acceder a ella hasta que el primero salga ejecutando `mutex.signal()`.


---

## Caso 4

Asumiendo que cada proceso se ejecuta en un único Thread:

Definimos en memoria compartida:

```c
permisoA = sem(1)
permisoB = sem(0)
permisoC = sem(0)
```

El código del proceso A va a ser:

```c
proximoAEjecutar = 'B'; // obs que no hace falta que sea atomica ni usar un mutex porque el unico proceso que la usa es el A y el A se ejecuta en un unico Thread
while(1){
    permisoA.wait();
    print("a");
    if(proximoAEjecutar == 'B'){
        proximoAEjecutar = 'C';
        permisoC.signal();
    }
    else{
        proximoAEjecutar = 'B';
        permisoB.signal(); // Se podrian hacer dos signals aca y sacar el cantidadDeEjecuciones del codigo de B.
    }
}
```

El código del proceso B va a ser:

```c
cantidadDeEjecuciones = 0; // obs que no hace falta que sea atomica ni usar un mutex porque el unico proceso que la usa es el B y el B no se ejecuta en mas de un Thread a la vez. 
while(1){
    permisoB.wait();
    print("b");
    cantidadDeEjecuciones++;
    if(cantidadDeEjecuciones == 2){
        cantidadDeEjecuciones = 0;
        permisoA.signal();
    }
    else{
        permisoB.signal();
    }
}
```

Y el del proceso C:

```c
while(1){
    permisoC.wait();
    print("c");
    permisoA.signal();
}
```
