## Punto A

Cuando usamos **Best Fit** para asignar memoria, asignamos siempre el primer hueco más chico que sea suficientemente grande como para contener la memoria que queremos asignar.

Teniendo disponibles para usar particiones de: `8 MB, 1 MB, 4 MB, 512 KB, 512 KB y 2 MB`
Y recibiendo las solicitudes en orden de: `500 KB, 6 MB, 3 MB, 20 KB, 4 MB`

#### Solicitud de `500 KB`
En los dos bloques que mejor entran porque sobra la menor cantidad de espacio es en alguno de los dos de `512 KB`. Así que lo asignamos al primero de los dos. Nos quedan disponibles para usar: `8 MB, 1 MB, 4 MB, 12 KB, 512 KB y 2 MB`.

#### Solicitud de `6 MB`
En el único bloque que entra es en el de `8 MB`. Así que lo asignamos ahí. 
Nos quedan disponibles: `2 MB, 1 MB, 4 MB, 12 KB, 512 KB y 2 MB`.

#### Solicitud de `3 MB`
El único bloque que nos sirve es el de `4 MB`, así que queda: `2 MB, 1 MB, 1 MB, 12 KB, 512 KB y 2 MB`

#### Solicitud de `20 KB`
El hueco más chico que lo puede contener es el de `512 KB`, así que va ahí. Nos quedan disponibles bloques de: `2 MB, 1 MB, 1 MB, 12 KB, 492 KB y 2 MB`

#### Solicitud de `4 MB`
Como no tenemos un bloque que tenga capacidad suficiente para almacenarlo, no podemos asignarle ninguno.

La cantidad de memoria desperdiciada son `4 MB + 504 KB`, que son aprox `4.492 MB`.

## Punto B

### Usando Worst Fit

Cuando usamos **Worst Fit**, asignamos siempre al bloque que pueda contener la solicitud lo más grande disponible.

#### Solicitud de `500 KB`
El hueco más grande que puede contenerlo es el de `8 MB`, así que lo mandamos ahí y nos quedan: `7.514 MB, 1 MB, 4 MB, 512 KB, 512 KB y 2 MB`

#### Solicitud de `6 MB`
En el único lugar que entra es en el primero, así que nos quedan: `1.514 MB, 1 MB, 4 MB, 512 KB, 512 KB y 2 MB`

#### Solicitud de `3 MB`
El único bloque posible es el de `4 MB`, así que va ahí: `1.514 MB, 1 MB, 1 MB, 512 KB, 512 KB y 2 MB`

#### Solicitud de `20 KB`
El bloque más grande que tenemos es el de `2 MB`, así que lo ubicamos ahí, quedándonos: `1.514 MB, 1 MB, 1 MB, 512 KB, 512 KB y 1.980 MB`

#### Solicitud de `4 MB`
No tenemos ningún bloque que pueda contener `4 MB`, así que siamo fuori.

### Usando First Fit

**First Fit** manda la solicitud al primer bloque lo suficientemente grande que pueda contener la solicitud.

#### Solicitud de `500 KB`
El primer bloque que lo puede contener es el de `8 MB`, así que nos queda el mismo primer paso que usando Worst Fit: `7.514 MB, 1 MB, 4 MB, 512 KB, 512 KB y 2 MB`.

#### Solicitud de `6 MB`
De nuevo, en el único lugar que entra es en el primero, así que nos quedan: `1.514 MB, 1 MB, 4 MB, 512 KB, 512 KB y 2 MB`.

#### Solicitud de `3 MB`
El único bloque disponible que lo puede contener es el de `4 MB`, así que ya a esta altura sabemos que no vamos a poder ubicar la última solicitud. A esta altura nos quedan los bloques: `1.514 MB, 1 MB, 1 MB, 512 KB, 512 KB y 2 MB`.

#### Solicitud de `20 KB`
El primer bloque lo suficientemente grande para poder almacenarlo es el primero, así que va allí: `1.495 MB, 1 MB, 1 MB, 512 KB, 512 KB y 2 MB`.

#### Solicitud de `4 MB`
No tenemos ningún bloque lo suficientemente grande para almacenarlo.

Con ninguna de las tres estrategias de asignación podemos ejecutar los 5 programas a la vez.

### Punto C

Suponiendo que hacer el uso más eficiente de la memoria es desperdiciar la menor cantidad de memoria posible, porque poder hacer que los cinco programas a la vez se puedan ejecutar no lo logró ninguno, la cantidad desperdiciada de cada estrategia fue:

- `Best Fit`: `6.4922 MB`
- `Worst Fit`: `6.494 MB`
- `First Fit`: `6.495 MB`

Así que prácticamente los tres desperdician la misma cantidad de memoria libre, la cual nos serviría para almacenar lo necesario para ejecutar el último proceso. Así que ninguna de las tres es más eficiente en este caso concreto.
