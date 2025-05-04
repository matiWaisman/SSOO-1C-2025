$$
\text{Waiting time promedio} = \frac{0 + 7 + 0 + 9 + 0}{5} = 3.2
$$

$$
\text{Turnaround promedio} = \frac{3 + 13 + 4 + 14 + 2}{5} = 7.2
$$

El scheduler es de prioridades preemptive, en particular uno Shortest Job First. 

Porque primero al no haber ningun otro proceso para ejecutar ejecuta el primero. Cuando llega el segundo proceso como al proceso 1 le falta un milisegundo de ejecución versus los 6 del proceso 2 sigue ejecutando el primero. Una vez que termina ejecuta el segundo porque es el unico otro proceso. Al milisegundo 4 llega el proceso 3, y como para terminar necesita menos tiempo que el proceso 2 se ejecuta a ese. Despues en el milisegundo 6 llega el proceso 4, pero como necesita más tiempo que el tres se sigue ejecutando este. Al terminar de ejecutarse el proceso 3 llega el 5. Y a partir de ese momento se ejecuta por orden de quien va a tardar menos en terminar. 