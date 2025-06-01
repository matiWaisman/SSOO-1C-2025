- Si _"Es importante que puedan crearse enlaces simbólicos"_:

  En un `File System` basado en `FAT` no se pueden crear enlaces simbólicos. En cambio, en uno basado en `inodos` sí.  
  Por lo tanto, basado en este requerimiento, conviene usar un sistema basado en `inodos`.

- Si _"Es importante que la cantidad de sectores utilizados para guardar estructuras auxiliares sea acotada, independientemente del tamaño del disco"_:

  En un `File System` de tipo `FAT`, el tamaño de la tabla FAT está determinado por la cantidad total de bloques del disco.  
  Por lo que dependiendo del tamaño del disco el tamaño de la tabla puede crecer. 
  En cambio un `File System` basado en `inodos` no dependen del tamaño del disco, y tienen un tamaño acotado por el tamaño de los bloques de datos y el direccionamiento. Por lo tanto es mejor elegir un `File System` basado en `inodos`.

- Si _"Es importante que el tamaño máximo de archivo sólo esté limitado por el tamaño del disco"_:

  Con `inodos`, la longitud de un archivo está limitada por la cantidad de bloques que puede direccionar un inodo mediante sus punteros directos e indirectos (simples, dobles, triples, etc.).

  En cambio, en `FAT`, un archivo puede ocupar cualquier cantidad de bloques encadenados mientras quepan en la tabla, la cual, en total, puede direccionar más bloques que un solo inodo.

  Por lo tanto, para este requerimiento, conviene un enfoque basado en `FAT`.

- Si _"Es importante que la cantidad de memoria principal ocupada por estructuras del filesystem en un instante dado sea (a lo sumo) lineal en la cantidad de archivos abiertos en ese momento"_:

  En un `File System` basado en `FAT`, incluso si no hay archivos abiertos, es necesario tener cargada en memoria toda la tabla FAT.  
  En cambio, en un sistema basado en `inodos`, solo es necesario cargar en memoria los inodos de los archivos que están en uso.

  Así, la cantidad de memoria ocupada en FAT es constante, mientras que en `inodos` es lineal con respecto a la cantidad de archivos abiertos.

  Por lo tanto, para este requerimiento, conviene un sistema basado en `inodos`.
