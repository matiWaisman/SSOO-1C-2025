Entiendo que con esta modificación se puede encolar varias veces un proceso en cualquier orden, por ejemplo: P1, P1, P2, P1, P3, P2, P3, P4. 

### Punto A

Esta modificación puede hacer que haya procesos con mayor tiempo de ejecución para que los que hacen más uso de CPU tengan períodos de tiempo más largos para ejecutarse. Si es prioritario para nuestro sistema que ese proceso más CPU-demandante termine rápido, puede ser útil. 

También se me ocurre que, si tenemos un proceso interactivo prioritario del cual podemos determinar en cuántos quantums puede resolver su conflicto de entrada y salida, podríamos lograr que se pueda ejecutar con el menor *Waiting Time* posible.

Así que, dependiendo de cómo se organice la lista de procesos *ready*, se le puede dar más prioridad a procesos interactivos poco CPU-demandantes o beneficiar a procesos más CPU-demandantes. Haciendo que no haya *starvation*, y que todos los procesos tengan *Waiting Times*, *Response Times* y *Turnarounds* razonables. 

Aunque estos casos dependerían de estar muy seguros de cuántos quantums van a tardar en terminar nuestros procesos para poder elegir un orden óptimo, por lo cual parecen muy teóricas sus ventajas. 

### Punto B

Esta modificación puede:

- Dependiendo del orden de los procesos, podría aumentar o disminuir el *throughput* a partir de un punto.  
- Aumentar o disminuir el *turnaround* dependiendo del orden de ejecución. Si se decide que es más importante que primero termine un proceso más largo, va a aumentar; si se decide encolar primero a muchos procesos cortos, podría disminuir.
- Dependiendo del orden, podría aumentar o disminuir el *Waiting Time*, igual que en el punto anterior. 
- Aumentar el *running time* porque se pueden encolar muchas veces los mismos procesos y tarda mucho en llegarle el primer quantum a un proceso. 

### Punto C

Para poder darle más tiempo de ejecución a tareas más CPU-demandantes, podríamos tener quantums de ejecución variables, lo cual está bueno versus este esquema porque nos ahorra el tiempo del cambio de contexto.
