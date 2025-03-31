Las syscalls proveen una interfaz directa entre un programa corriendo a nivel usuario y los servicios brindados por el sistema operativo. Por medio de las syscalls un programa a nivel usuario puede acceder a funcionalidades del kernel sin tener que estar corriendo en nivel 0. 

En cambio una función de una bibloteca lo que hace es facilitar tareas comunes encapsulandolas. Por más que una función de una bibloteca pueda invocar una syscall, su funcionamiento no es proporcionado en su totalidad por el kernel. 

Las biblotecas pueden incluso servir como una abstracción a las syscalls. Por ejemplo la funcion malloc de la libc usa syscalls pero todo su funcionamiento no es proveido por el kernel. 
