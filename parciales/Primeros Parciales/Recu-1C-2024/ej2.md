| Tiempo    |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  |
|-----------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
| Proceso   |  1  |  1  | CC  |  3  |  3  |  3* | CC  |  1* | CC  |  4  | CC  |  2  |  2  |  2  |  2  |  2* | CC  |  4  |  4* |
| Bloqueado |     |     | P1  | P1  | P1  |     |     |     |     |     | P4  | P4  |     |     |     |     |     |     |     |


El `Waiting Time` promedio es: `(2 + 11 + 2 + 9)/4 = 6`

El `Turnaround` promedio es: `(8 + 16 + 5 + 14)/4 = 10.75`

Como `SJF` es un *scheduler* de prioridades estáticas, y todos los *schedulers* de prioridades estáticas llevan a inanición, este no está exento. La inanición en los *schedulers* de prioridades estáticas ocurre si constantemente ingresan procesos de mayor prioridad al sistema: todos los que tienen una prioridad menor no se van a poder ejecutar. En el caso de que la prioridad sea `SJF`, si constantemente llegan procesos cortos, los procesos más largos no se van a poder ejecutar nunca.

Para solucionarlo, habría que hacer que las prioridades dejen de ser estáticas y usar un sistema `Multi Level Feedback Queue`, donde los procesos más cortos tengan más prioridad, pero que los más largos tengan *aging* para poder ejecutarse. Como no queremos que un proceso largo se ejecute por completo si sube de cola (porque eso empeoraría el *response time* de los procesos cortos, que probablemente sean interactivos), habría que hacer que la cola de procesos interactivos de mayor prioridad sea *round robin*. (Habría que guitarrear más).
