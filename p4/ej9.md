El algoritmo `FIFO` va a reemplazar el marco de página 3, ya que fue el primero en ser cargado (110).

El algoritmo `LRU` va a reemplazar el marco de página 1, ya que es al que hace más tiempo que no se usa (265).

El algoritmo `Second Chance` va a reemplazar el marco 2, porque es el que hace más tiempo fue cargado y no ha sido referenciado desde la última vez que chequeamos. El marco 3 fue cargado antes, pero como fue referenciado desde la última vez que tuvimos que reemplazar, se gana su *second chance*.
