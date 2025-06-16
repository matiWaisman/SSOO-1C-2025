## Punto A

Para una pantalla tactil de un celular con Android querriamos que la interacción con el usuario por medio de la pantalla tactil sea inmediata, por lo que tiene sentido hacer un buffer ni para las interacciones del usuario con la pantalla ni para devolverle los resultados al usuario. 

Para el disco virtual tiene mucho sentido tener un buffer para almacenar una mayor cantidad de entradas del usuario. Porque si el sistema va subiendo los archivos por red a otro lado va a tardar mucho y no cae en el mismo caso del anterior que se resuelven las solicitudes rapidamente. Por lo tanto es una buena idea tener un buffer para ir guardando los archivos que se van cargando mientras se está subiendo otro archivo por internet. 

Mismo caso de realizar operaciones que tardan mucho la GPU. Como hacer multiplicaciones con matrices puede tomar mucho tiempo y la GPU puede estar realizando muchas tareas en el medio tambien hace que tenga mucho sentido tener un buffer para ir guardando la entrada del usuario. 

## Punto B
Para el celular: Siendo que el usuario no está absolutamente todo el tiempo tocando la pantalla, que son rapidas de resolver las acciones y que es fundamental en el sistema que las interacciones con la pantalla se resuelvan rapido me parece que las interrupciones son la mejor opción.

Como en el disco virtual se quieren transferir grandes volumenes de datos y lo normal no es que se carguen todo el tiempo archivos si no que se cargan relativamente pocos y que pesan mucho polling queda completamente descartado. Por lo tanto DMA y interrupciones son mejores opciones. 

Dependiendo del uso que le damos a este disco virtual, si el sistema es un servidor o una computadora hogareña y queremos tener un disco virtual chico va a convenir más DMA o interrupciones.

Si es un disco chico para un uso hogareño no tiene sentido tener una pieza de Hardware especifica en nuestra compu asi que por lo tanto es mejor interrupciones.

Si es la infraestructura de un servidor con un disco virtual muy grande va a ser mejor DMA porque está pensado para casos como este que se necesita procesar un gran volumen de datos sin que intervenga la cpu. 

Para la GPU por lo mismo que el disco queda descartado polling. Y si es una computadora que se usa para calculos cientificos va a manejarse un gran volumen de datos y es necesario que sea lo más eficiente posible y se puede dar el lujo de tener hardware necesario. Por lo tanto va a ser mejor DMA.

