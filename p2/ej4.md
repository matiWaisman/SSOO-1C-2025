### Round Robin
Si es un RR sin prioridades no puede haber starvation porque todos los procesos reciben tiempo de cpu tarde o temprano. Aunque si puede disminuir el Throughput y aumentar el Turnaround, Waiting Time y Response time de los procesos si no les ponemos prioridades. 

### Por Prioridad.
Las prioridades estaticas llevan a starvation, porque si tenemos un proceso con menor prioridad y siempre se agregan al pool de ejecucion procesos con una mayor prioridad nuesta tarea de menor prioridad nunca va a ser ejecutada porque siempre va a existir una con mayor prioridad esperando a ejecutarse. 

Lo mismo si tenemos muchas procesos interactivos que usan poco el cpu y se bloquean por una E/S que se resuelve rapido estos procesos se van a estar ejecutando siempre mientras el proceso con menor prioridad va a seguir esperando. 

### Shortest Job First
Este tipo de Scheduler es un caso particular del esquema de prioridades. Tambien puede llevar a starvation si tenemos muchos procesos cortos. 

### Shortest Remaining Time First
Lo mismo que SJF.


### First In First Out
Si tenemos muchos procesos de uso de la CPU intensivos y otros de menor uso interactivos los procesos interactivos tendrían un Response Time altisimo, aunque no entra en la definición de starvation porque teoricamente tarde o temprano les va a tocar el cpu.

### Colas de Multinivel
Al ser una implementación de prioridades estaticas sigue el problema mencionado anteriormente. Si tenemos procesos con menor prioridad y se siguen agregando procesos con mayor prioridad por más que los procesos prioritarios tengan un menor Waiting Time a los procesos menos prioritarios nunca les llega el cpu. Por lo que hay starvation. 

### Colas de multinivel con feedback (aging)
Dependiendo como sea el criterio dentro de cada cola, y los criterios para aumentar y disminuir la prioridad de los procesos si se puede resolver que no haya starvation. 
Si tuvieramos un criterio para hacer que aumenten su prioridad los procesos que no son ejecutados para que puedan entrar en la cola menos prioritaria ahi podríamos no tener starvation.

Tambien depende del algoritmo de scheduling de cada cola, en particular si dentro de la de nivel 0 usamos SJF tenemos el mismo problema que solo usando SJF, por lo que habría starvation. 

