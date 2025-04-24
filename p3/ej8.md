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
cantidadEjecucionesB = 0;
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
cantidadConsumidores = 0
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
proximoAEjecutar = 'B';
while(1){
    permisoA.wait();
    print("a");
    if(proximoAEjecutar == 'B'){
        proximoAEjecutar = 'C';
        permisoC.signal();
    }
    else{
        proximoAEjecutar = 'B';
        permisoB.signal();
    }
}
```

El código del proceso B va a ser:

```c
cantidadDeEjecuciones = 0;
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
