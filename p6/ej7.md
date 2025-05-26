- Si _"Es importante que puedan crearse enlaces simbólicos"_:

  En un `File System` basado en `FAT` no se pueden crear enlaces simbólicos. En cambio, en uno basado en `inodos` sí.  
  Por lo tanto, basado en este requerimiento, conviene usar un sistema basado en `inodos`.

- Si _"Es importante que la cantidad de sectores utilizados para guardar estructuras auxiliares sea acotada, independientemente del tamaño del disco"_:

  En un `File System` de tipo `FAT`, el tamaño de la tabla FAT está determinado por la cantidad total de bloques del disco.  
  Aunque pueda ocupar varios sectores, su tamaño es fijo, por lo tanto, está acotado y no crece con la cantidad de archivos o el uso del sistema.

  En cambio, en un sistema basado en `inodos`, cada archivo tiene un inodo con posibles bloques de punteros directos, indirectos, dobles, etc.  
  Esto significa que el uso de estructuras auxiliares (bloques de punteros) en disco puede crecer dinámicamente según la cantidad y el tamaño de los archivos.

  Por lo tanto, si el objetivo es mantener acotado el uso de sectores dedicados a estructuras auxiliares, conviene adoptar un diseño inspirado en `FAT`.

(CHEQUEAR, Si me dicen que nunca los inodos van a en memoria ser igual o mayores a un FAT entonces es mejor inodos)

- Si _"Es importante que el tamaño máximo de archivo sólo esté limitado por el tamaño del disco"_:

  Con `inodos`, la longitud de un archivo está limitada por la cantidad de bloques que puede direccionar un inodo mediante sus punteros directos e indirectos (simples, dobles, triples, etc.).

  En cambio, en `FAT`, un archivo puede ocupar cualquier cantidad de bloques encadenados mientras quepan en la tabla, la cual, en total, puede direccionar más bloques que un solo inodo.

  Por lo tanto, para este requerimiento, conviene un enfoque basado en `FAT`.

- Si _"Es importante que la cantidad de memoria principal ocupada por estructuras del filesystem en un instante dado sea (a lo sumo) lineal en la cantidad de archivos abiertos en ese momento"_:

  En un `File System` basado en `FAT`, incluso si no hay archivos abiertos, es necesario tener cargada en memoria toda la tabla FAT.  
  En cambio, en un sistema basado en `inodos`, solo es necesario cargar en memoria los inodos de los archivos que están en uso.

  Así, la cantidad de memoria ocupada en FAT es constante, mientras que en `inodos` es lineal con respecto a la cantidad de archivos abiertos.

  Por lo tanto, para este requerimiento, conviene un sistema basado en `inodos`.
