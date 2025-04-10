Como $$P_{0}$$ tiene rafágas cortas de E/S y $$P_{1}$$ frecuentemente se bloquea leyendo de la red podemos asumir que ambos son procesos interactivos los cuales no podemos dejar colgados sin que se ejecuten una vez que están ready porque $$P_{2}$$ se está ejecutando. Si no para los usuarios estos procesos no se sentirían responsivos. 

En cambio $$P_{2}$$ parece ser un programa de tipo batch que calcula muchas cosas y luego lo escribe al disco. Por lo que no darle el tiempo de ejecución de corrido no debería afectarle al usuario. 

Por lo tanto usar un algoritmo de Round Robin me parece una buena idea porque haría que se ejecuten los primeros dos procesos frecuentemente, y si el tiempo por quantum está bien elegido podría hacer que en un quantum $$P_{0}$$ y $$P_{1}$$ hagan todo lo necesario y se bloqueen. Dejandole quantums de más al proceso 2 para que se ejecute. 

Se podría elegir un sistema de prioridades en el cual los primeros dos procesos sean prioritarios y entre ellos se decida en forma Round Robin y si ninguno de los dos procesos está listo se ejecute el tercero hasta que alguno de los 2 esté ready, aunque habría que tener cuidado que eso no genere starvation en el tercer proceso. 



