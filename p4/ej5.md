Para toda política de reemplazo, si tenemos un solo frame, cada vez que haya una referencia a una página vamos a tener un page fault, porque en la secuencia del ejercicio nunca se accede dos veces seguidas a la misma página. Así que habrá 20 page faults.
Sea cual sea la política, como solo podemos tener una página en memoria a la vez, siempre vamos a reemplazarla si la que tenemos que leer no es la que tenemos en memoria actualmente.

Por otro lado, para cualquier política de reemplazo, si tenemos 7 frames, solo habrá un page fault al cargar por primera vez cada página, pero nunca será necesario reemplazar una página, ya que tenemos frames suficientes para todas.
Por lo tanto, una vez que cargamos una página por primera vez, cada vez que la necesitemos será un hit asegurado. Así que solo habrá 7 page faults.

Realizo las tablas solo para los casos en que tenemos 4 frames.

### Usando LRU

| Página pedida | Frames | Páginas a desalojar |
|---------------|---------------------------|---------------------|
| 1             | [<span style="color:red">1</span>, _, _, _] | 1                   |
| 2             | [1, <span style="color:red">2</span>, _, _] | 1 2                 |
| 3             | [1, 2, <span style="color:red">3</span>, _] | 1 2 3               |
| 4             | [1, 2, 3, <span style="color:red">4</span>] | 1 2 3 4             |
| 2             | [1, <span style="color:green">2</span>, 3, 4] | 1 3 4 2             |
| 1             | [<span style="color:green">1</span>, 2, 3, 4] | 3 4 2 1             |
| 5             | [<span style="color:red">5</span>, 2, 3, 4] | 4 2 1 5             |
| 6             | [5, <span style="color:red">6</span>, 3, 4] | 2 1 5 6             |
| 2             | [5, 6, <span style="color:red">2</span>, 4] | 1 5 6 2             |
| 1             | [<span style="color:red">1</span>, 6, 2, 4] | 5 6 2 1             |
| 2             | [1, 6, <span style="color:green">2</span>, 4] | 5 6 1 2             |
| 3             | [<span style="color:red">3</span>, 6, 2, 4] | 6 1 2 3             |
| 7             | [3, <span style="color:red">7</span>, 2, 4] | 1 2 3 7             |
| 6             | [3, 7, 2, <span style="color:red">6</span>] | 2 3 7 6             |
| 3             | [<span style="color:green">3</span>, 7, 2, 6] | 2 7 6 3             |
| 2             | [3, 7, <span style="color:green">2</span>, 6] | 7 6 3 2             |
| 1             | [<span style="color:red">1</span>, 7, 2, 6] | 6 3 2 1             |
| 2             | [1, 7, <span style="color:green">2</span>, 6] | 6 3 1 2             |
| 3             | [1, 7, <span style="color:red">3</span>, 6] | 6 1 2 3             |
| 6             | [1, 7, 3, <span style="color:green">6</span>] | 1 2 3 6             |

En total hay 10 Page Faults.

### Usando FIFO 

| Página pedida | Frames después de cargar | Cola de desalojos (de primero a último) |
|---------------|---------------------------|-----------------------------------------|
| 1             | [<span style="color:red">1</span>, _, _, _] | 1 |
| 2             | [1, <span style="color:red">2</span>, _, _] | 1 2 |
| 3             | [1, 2, <span style="color:red">3</span>, _] | 1 2 3 |
| 4             | [1, 2, 3, <span style="color:red">4</span>] | 1 2 3 4 |
| 2             | [1, <span style="color:green">2</span>, 3, 4] | 1 2 3 4 |
| 1             | [<span style="color:green">1</span>, 2, 3, 4] | 1 2 3 4 |
| 5             | [<span style="color:red">5</span>, 2, 3, 4] | 2 3 4 5 |
| 6             | [5, <span style="color:red">6</span>, 3, 4] | 3 4 5 6 |
| 2             | [5, 6, <span style="color:red">2</span>, 4] | 4 5 6 2 |
| 1             | [5, 6, 2, <span style="color:red">1</span>] | 5 6 2 1 |
| 2             | [5, 6, <span style="color:green">2</span>, 1] | 5 6 2 1 |
| 3             | [<span style="color:red">3</span>, 6, 2, 1] | 6 2 1 3 |
| 7             | [3, <span style="color:red">7</span>, 2, 1] | 2 1 3 7 |
| 6             | [3, 7, <span style="color:red">6</span>, 1] | 1 3 7 6 |
| 3             | [<span style="color:green">3</span>, 7, 6, 1] | 1 3 7 6 |
| 2             | [3, 7, 6, <span style="color:red">2</span>] | 3 7 6 2 |
| 1             | [<span style="color:red">1</span>, 7, 6, 2] | 7 6 2 1 |
| 2             | [1, 7, 6, <span style="color:green">2</span>, ] | 7 6 2 1 |
| 3             | [1,<span style="color:red">3</span>, 6, 2] | 6 2 1 3 |
| 6             | [1, 3, <span style="color:red">6</span>, 2] | 6 2 1 3 |

En total hay 14 Page Faults.

### Usando Second Chance (Tengo que pasarlo a MarkDown)

![alt text](tabla5.png)

En total hay 10 Page Faults


