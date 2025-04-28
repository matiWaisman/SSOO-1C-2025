Decimos que hay **trashing** cuando la memoria disponible es insuficiente y los procesos compiten excesivamente por ella. Como resultado, el sistema operativo pasa mucho tiempo realizando page swaps entre la memoria principal y el disco, lo que provoca un desperdicio en el uso del CPU. Esto ocurre porque se bajan páginas a disco y se leen desde él constantemente, y estos procedimientos son muy lentos.

El trashing suele ocurrir cuando hay poca memoria disponible y/o cuando hay demasiados procesos intentando acceder a la memoria. Además, un mal sistema de reemplazo de páginas contribuye al problema, ya que constantemente se cargan a disco páginas de procesos para luego tener que traerlas nuevamente a la memoria, lo que empeora aún más el rendimiento del sistema.

Puede suceder que como el CPU no está siendo utilizado lo suficiente porque está constantemente leyendo y guardando cosas en disco, el scheduler vea este comportamiento y le agregue más procesos para usar más el CPU. 
Como estos nuevos procesos también tienen el problema de que todo el tiempo están recibiendo page swaps sus páginas, el scheduler sigue agregando procesos a la cola, hasta que colapse el sistema...

Si cambiamos el sistema:

#### Instalando una CPU Más Rápida
La CPU seguiría siendo desaprovechada porque seguiríamos haciendo trashing al tener poca memoria y/o una mala política de reemplazo de páginas y nos la pasaríamos reemplazando páginas en vez de ejecutar procesos.

Ya de por sí el uso del CPU es bajo, así que aumentar la velocidad no ayuda a solucionar el problema del trashing.

#### Instalando un disco de paginado más grande
-

#### Incrementando el grado de multiprogramación
Sabiendo que el sistema está haciendo trashing, agregar más procesos a la cola llevaría a la **catástrofe**. Porque estos procesos nuevos que se agregan no van a tener espacio en memoria para ejecutarse y se van a pelear con los procesos que ya estaban haciendo trashing haciendo page swaps entre sí constantemente. Como el uso del CPU va a seguir bajo, el scheduler puede decidir seguir incrementando el grado de multiprogramación, aumentando la cantidad de page faults y va a seguir incrementando el problema constantemente.

#### Disminuyendo el grado de multiprogramación
Suponiendo que todos los procesos necesitan más o menos la misma cantidad de páginas en memoria para poder ejecutarse, algo bastante alejado de la realidad, este cambio puede ayudar a disminuir la cantidad de page faults, porque al haber menos procesos tratando de ejecutarse va a hacer falta una menor cantidad de page swaps al querer ejecutar un proceso.

Que haya menos page faults disminuiría/eliminaría el trashing y aumentaría el uso de la CPU porque hay más probabilidades de que al querer ejecutar un proceso lo que necesitamos para ejecutarlo ya esté presente en memoria principal y no haya que hacer una búsqueda a disco/page swap. Así que es un buen cambio para el sistema.

Si los procesos que eliminamos de la cola son procesos que usan pocas páginas y/o las escriben poco y dejamos los que más páginas precisan y más las escriben, seguiríamos con el mismo problema, pero suponemos que todos hacen y precisan más o menos lo mismo para concluir que disminuye la cantidad de page faults.

#### Instalando más memoria principal
Este cambio también puede ayudar a disminuir la cantidad de page faults llevando a los mismos beneficios mencionados en el punto anterior, así que también es un buen cambio para el sistema.

Habría menos page faults porque al tener más memoria principal podemos tener presentes en memoria más páginas, disminuyendo las probabilidades de que al ejecutar un proceso sus páginas no estén cargadas en memoria todavía.

#### Instalando un disco más rápido
Con este cambio podemos hacer que el mecanismo de los page faults y swaps se resuelva más rápido, pero la cantidad de estos va a seguir siendo la misma por lo que el trashing va a seguir existiendo. Por lo que es un cambio que ayuda pero no tanto como los dos anteriores.

#### Incrementando el tamaño de las páginas
Incrementar el tamaño de las páginas puede llevar a tener mayor fragmentación interna. Como nuestro problema es que tenemos poca memoria y/o una mala política de reemplazo, que las páginas sean más grandes no nos da ningún beneficio e incluso puede llevar a más problemas, así que no es un cambio producente.

#### Incrementando la velocidad del bus de E/S
Como el uso del bus de E/S está solo al 10% no va a cambiar nada para aumentar el uso del CPU y disminuir el trashing.
