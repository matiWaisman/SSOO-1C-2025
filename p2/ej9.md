## Punto A, Round Robin con quantum de 5

| Tiempo   |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  |
|----------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
| Proceso  |  P1  |  P1  |  P1  |  P1  |  P1  | P2 | P2  | P2 | P2  | P2  | P1  | P1  | P1*  | P2  | P2  | P2*  | P3 | P3  | P3  | P3  | P3  | P4  | P4  | P4  | P4  | P4*  | P3* |

## Punto B, Shortest Remaining Time First

| Tiempo   |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  |
|----------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
| Proceso  |  P1  |  P1  |  P1  |  P1  |  P1  | P1 | P1  | P1* | P2  | P2  | P2  | P2  | P2  | P2  | P2  | P2*  | P4 | P4  | P4  | P4  | P4*  | P3  | P3  | P3  | P3  | P3  | P3* |

## Punto C

$$
\text{Turnaround promedio RR} = \frac{13 + 11 + 12 + 10}{4} = 11.5
$$
$$
\text{Turnaround promedio SRTF} = \frac{8 + 10 + 12 + 5}{4} = 8.75
$$

## Punto D 

Por más que Shortest Remaining Time First tiene un menor Turnaround, en este caso, que Round Robin en el caso de tener un sistema en el cual se van agregando procesos cortos muy seguido usar SRTF puede generar Starvation de procesos más largos como procesos Batch. Y tambien si tenemos muchos procesos interactivos o real-time el tener que esperar tanto para poder ejecutarse genera que no se sientan responsivos esos procesos. 

Tambien en la practica no es fácil estimar el tiempo restante de un proceso para usar SRTF. 
