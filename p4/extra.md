Preguntas clase repaso:

A) Si tenemos 4 marcos de página y más de 6 pedidos de página, para cualquier orden que nos llegue, Second
Chance siempre es mejor que LRU.

`LRU` utiliza el atributo de cuál página fue usada hace más tiempo en memoria.

`Second Chance` utiliza el atributo de cuál página ha estado más tiempo en memoria y no haya sido referenciada desde la última vez que tuvimos que hacer un reemplazo.

Lo que tiene de bueno `Second Chance` es que es más fácil de implementar que `LRU`. Porque no hace falta rastrear el momento de uso de cada página. Solamente hay que chequear un bit. 

Pero si lo que quiere el ejercicio es una secuencia de accesos a página que difiera según los algoritmos, lo hago. 

Así que, como contraejemplo, la secuencia: 1,2,3,4,1,4,5,6,1,2,5. 

En esa secuencia `LRU` tiene 7 page faults y `Second Chance` 8. 

B) Thrashing se elimina aumentando los marcos de página.

Ya que, al final del día, la memoria física es limitada y no podemos controlar cuánta memoria van a querer los procesos que ejecutamos, nunca se puede con certeza eliminar el Trashing. Lo que sí puede hacerse es mitigarlo/reducirlo. 

Aumentando la cantidad de páginas disponibles en memoria (i.e. agregar más memoria) es una buena mejora para mitigar el trashing, porque lo que lograría es disminuir la cantidad de page faults, ya que al tener más lugar para almacenar páginas, hay más chances de que la página que queremos usar esté presente en memoria y no fue desalojada todavía.

Que haya menos page faults disminuiría/mitigaría el trashing y aumentaría el uso de la CPU porque hay más probabilidades de que, al querer ejecutar un proceso, lo que necesitamos para ejecutarlo ya esté presente en memoria principal y no haya que hacer una búsqueda a disco/page swap.
