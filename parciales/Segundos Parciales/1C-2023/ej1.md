La idea de la función va a ser ir recorriendo los directorios dentro de la ruta del directorio que pasan como parametro. Si me encuentro con un dir entry que es un directorio llamo recursivamente a la función con el directorio concatenado. Si es un archivo llamo a otra función que va a recorrer los bloques de datos del archivo y por cada vez que encuentre la palabra en una línea lo va a printear.

```c
void my_grep(char* palabra, char* path){
    struct Ext2FSInode * inodo_actual = inode_for_path(path);
	unsigned int cantidad_de_bloques_en_dir = inodo_actual->size / BLOCK_SIZE; 
	if(inodo_actual->size % BLOCK_SIZE != 0) {
		cantidad_de_bloques_en_dir++;
	}

	unsigned int bytes_leidos = 0;
	unsigned char *bloques = (unsigned char *) malloc(BLOCK_SIZE*2);

	unsigned int left_block_address = get_block_address(from,0);
	read_block(left_block_address,bloques);

	for(int nro_bloque=0; nro_bloque<cantidad_de_bloques_en_dir; nro_bloque++) {

		if(nro_bloque != 0) {
			memcpy(bloques,bloques+BLOCK_SIZE,BLOCK_SIZE);
		}
		if(nro_bloque != cantidad_de_bloques_en_dir-1) {
			unsigned int right_block_address = get_block_address(from,nro_bloque+1);
			read_block(right_block_address,bloques+BLOCK_SIZE);
		} else {
			// Achico la memoria que no se usa
			bloques = (unsigned char *) realloc(bloques,BLOCK_SIZE);
		}

		unsigned char *dirEntryBytes = bloques;

		if(bytes_leidos >= BLOCK_SIZE) {
			bytes_leidos = bytes_leidos - BLOCK_SIZE;
			dirEntryBytes += bytes_leidos;
		}


		while(bytes_leidos<BLOCK_SIZE) {

			struct Ext2FSDirEntry *dirEntry = (struct Ext2FSDirEntry *) dirEntryBytes;

			if(dirEntry->file_type == 2){
                my_grep(palabra, path + "/" + dir_entry->name); // Es pseudocodigo loko, no pasa nada.
            }
            if(dirEntry->file_type == 1){
                grep_palabra_en_archivo(palabra, path + "/" + dir_entry->name); // Es pseudocodigo loko, no pasa nada.
            }

			bytes_leidos += (unsigned int) dirEntry->record_length;
			dirEntryBytes += dirEntry->record_length;
		}
	}
	free(bloques);
	return NULL;
}
```

```c
// Si o si el path es de un regular file, no puede ser un directorio 
grep_palabra_en_archivo(char * palabra, char * path){
    struct Ext2FSInode * inodo_actual = inode_for_path(path);
	unsigned int cantidad_de_bloques_en_dir = inodo_actual->size / BLOCK_SIZE; 
	if(inodo_actual->size % BLOCK_SIZE != 0) {
		cantidad_de_bloques_en_dir++;
	}

	unsigned int bytes_leidos = 0;
	unsigned char *bloques = (unsigned char *) malloc(BLOCK_SIZE*2);

	unsigned int left_block_address = get_block_address(from,0);
	read_block(left_block_address,bloques);

	for(int nro_bloque=0; nro_bloque<cantidad_de_bloques_en_dir; nro_bloque++) {

		if(nro_bloque != 0) {
			memcpy(bloques,bloques+BLOCK_SIZE,BLOCK_SIZE);
		}
		if(nro_bloque != cantidad_de_bloques_en_dir-1) {
			unsigned int right_block_address = get_block_address(from,nro_bloque+1);
			read_block(right_block_address,bloques+BLOCK_SIZE);
		} else {
			// Achico la memoria que no se usa
			bloques = (unsigned char *) realloc(bloques,BLOCK_SIZE);
		}

		unsigned char *bytesArchivo = bloques;

		if(bytes_leidos >= BLOCK_SIZE) {
			bytes_leidos = bytes_leidos - BLOCK_SIZE;
			bytesArchivo += bytes_leidos;
		}


		while(bytes_leidos<BLOCK_SIZE) {
            if(find(get_line(bytesArchivo), palabra)){
				printf("<%s> %s", path, get_line(bytesArchivo));
			}
			unsigned int long_linea = strlen(get_line(bytesArchivo))
			bytes_leidos += long_linea;
			bytesArchivo += (unsigned char) long_linea;
		}
	}
	free(bloques);
	return NULL;
}
```