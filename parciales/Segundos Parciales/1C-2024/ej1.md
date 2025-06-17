## Punto A

Si tenemos el directorio del inodo `A` y queremos acceder a los bloques de datos del archivo `A/B/c.txt` tenemos que: 

- Para esto podemos primero implementar una funcion que para un inodo devuelve un puntero al i-esimo bloque de datos, `load_inode`: 

  La funcion primero chequea si el numero de bloque de datos esta dentro de las entradas directas, si es asi simplemente habria que devolver `block[i]`. 

  Si no esta dentro de las entradas directas y esta dentro de una indireccion seria mas facil primero hacer la cuenta de en que indireccion esta, porque si no vamos a cargar bloques de disco sin ningun motivo.

  Llamo #bloques direccionados por indireccion a `BLOCK_SIZE / direccionamiento de bloque`.

  La condicion para determinar si el i-esimo bloque esta dentro de la primera indireccion seria `(i - #entradas directas) < #bloques direccionados por indireccion`. 

  Si esto es verdadero habria que primero cargar el bloque de la indireccion simple y despues devolver la direccion ubicada en la posicion `i - #entradas directas`. 

  La condicion para determinar si esta dentro de la indireccion doble seria `(i - #entradas directas - #bloques direccionados por indireccion) < (#bloques direccionados por indireccion)^2`. 

  Si esta dentro de la indireccion doble primero habria que cargar el bloque de la indireccion doble, y despues cargar el bloque de indireccion numero `(i - #entradas directas - #bloques direccionados por indireccion) / (#bloques direccionados por indireccion)` y devolver la entrada numero `i - #entradas directas - #bloques direccionados por indireccion mod #bloques direccionados por indireccion` de este bloque.

```c
struct Ext2FSInode* get_file(struct Ext2FSInode * from, char * arch_nombre){
    unsigned int cantidad_de_bloques_en_dir = from->size / BLOCK_SIZE; // Asumo que tengo una constante definida BLOCK_SIZE
	if(from->size % BLOCK_SIZE != 0) {
		cantidad_de_bloques_en_dir++;
	}

	unsigned int bytes_leidos = 0;
	unsigned char *bloques = (unsigned char *) malloc(BLOCK_SIZE*2);

	unsigned int left_block_address = get_block_address(from,0);
	read_block(left_block_address,bloques);

	for(int nro_bloque=0; nro_bloque<cantidad_de_bloques_en_dir; nro_bloque++) {

		// Uso memcpy para que sea mas rapido
		if(nro_bloque != 0) {
			memcpy(bloques,bloques+BLOCK_SIZE,BLOCK_SIZE);
		}
		if(nro_bloque == cantidad_de_bloques_en_dir-1) {
            // Achico la memoria que no se usa
			bloques = (unsigned char *) realloc(bloques,BLOCK_SIZE);
		} else {
			unsigned int right_block_address = get_block_address(from,nro_bloque+1);
			read_block(right_block_address,bloques+BLOCK_SIZE);
		}

		unsigned char *dirEntryBytes = bloques;

		if(bytes_leidos >= BLOCK_SIZE) {
			bytes_leidos = bytes_leidos - BLOCK_SIZE;
			dirEntryBytes += bytes_leidos;
		}


		while(bytes_leidos<BLOCK_SIZE) {

			struct Ext2FSDirEntry *dirEntry = (struct Ext2FSDirEntry *) dirEntryBytes;

			unsigned int max_length = std::max((size_t)dirEntry->name_length,strlen(filename));
			if(!strncmp(dirEntry->name,arch_nombre,max_length)) { // Si son iguales
				struct Ext2FSInode *inodo = load_inode(dirEntry->inode);
				free(bloques);
                if(!strcmp(arch_nombre, "c.txt")){ // Si estabamos explorando el directorio B devolvemos el inodo directamente
                    return inodo;
                }
                else{ //Si estabamos explorando el directorio A y encontramos B ahora buscamos el archivo c.txt.
                    return get_file(inodo, "c.txt");
                }
				
			}
			bytes_leidos += (unsigned int) dirEntry->record_length;
			dirEntryBytes += dirEntry->record_length;
		}
	}
	free(bloques);
	return NULL;
}
```

Y habría que llamar a la función como `get_file(a, "b")`. 

## Punto B
Para renombrar el archivo `/home/juan/listado.txt` a `/home/maria/archivo.txt` primero habria que recorrer desde el directorio raiz hasta `listado.txt` de manera similar al ejercicio anterior. Una vez que encontramos la direntry asociada a `listado.txt` hay que guardarnos el numero de inodo y eliminar la direntry del directorio de juan. 

Luego hay que recorrer desde el directorio raiz al directorio de maria y en el directorio de maria recorrer los bloques de datos del directorio y cuando encontremos un espacio disponible hacemos una escritura al disco para agregar una dir entry nueva llenando todos los campos, poniendole el nombre `archivo.txt` y poniendole como numero de inodo el que nos habiamos guardado previamente. 

Por lo que van a ser dos escrituras a disco, una para borrar la dir entry vieja y una para crear la dir entry nueva.

## Punto C
Como los sistemas basados en inodos permiten links simbolicos y hard links si un programa realiza una busqueda similar a la del punto A de manera recursiva, si no se tiene cuidado con los enlaces simbolicos puede facilmente ocurrir una recursion infinita si dentro de un directorio hay un simbolic link a un archivo de "mas abajo" del arbol de archivos y este archivo que esta mas bajo vuelve a apuntar al de arriba. Cuando querramos leer el contenido del archivo de mas arriba vamos a tener que ir a buscar el archivo de mas abajo, y cuando querramos leer el contenido del archivo de mas abajo vamos a tener que ir al de arriba, teniendo un ciclo infinito. 