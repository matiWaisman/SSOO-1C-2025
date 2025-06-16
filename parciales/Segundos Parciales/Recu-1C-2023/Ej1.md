## Punto A

Vamos primero con la version basada en `ext2`. La idea seria recorrer los bloques de datos del inodo `from` y crear bloques nuevos en `to` que tengan los mismos datos. 

```c
void my_copy(char * from, char * to){
    struct Ext2FSInode * inodo_from = inode_for_path(from);
    struct Ext2FSInode * inodo_to = inode_for_path(to);

    cantidad_bloques_a_recorrer = ceil(inodo_from->size/BLOCK_SIZE)
    char * buffer = malloc(BLOCK_SIZE); // Defino el buffer sobre el cual voy a hacer las copias

    for(int i = 0; i < cantidad_bloques_a_recorrer; i++){
        unsigned int block_address_actual_from = get_block_address(inodo_from, i);

        read_block(block_address_actual_from, buffer);

        // Conseguimos una direccion nueva para nuestro bloque en to
        unsigned int free_block_address_to = gett_free_block_address(inodo_to);

        write_block(free_block_address_to, buffer);
        add_block_address_to_inode(inodo_to, free_block_address_to, i);
    }
    inodo_to->size = inodo_from->size;

    free(buffer);
}
```

Ahora vamos con la version para `FAT`. Para esta solucion hay que ir recorriendo los bloques del archivo `from` y copiando esos bloques a nuevos bloques que vamos a ir agregando a la `FAT` para el archivo `to`.

```c
void my_copy(char * from, char * to){
    unsigned int current_block_from = get_init_block_for_path(from);
    unsigned int current_block_to = get_init_block_for_path(to);
    
    char * buffer = malloc(BLOCK_SIZE); // Defino el buffer sobre el cual voy a hacer las copias

    while(current_block_from != -1){
        read_block(current_block_from, buffer);
        write_block(current_block_to, buffer); 
        // Movemos ambos bloques
        current_block_from = FAT[current_block_from];
        // Actualizamos a donde apunta to
        if(current_block_from == -1){
            FAT[current_block_to] = -1; // Para indicar que termino el archivo y no hay ningun bloque de datos mas.
        }
        else{
            FAT[current_block_to] = get_free_block_address();
            current_block_to = FAT[current_block_to];
        }
    }

    free(buffer);
}
```

## Punto B
Para implementar `get_init_block_for_path()` en `FAT` habria hacer una recorrida por los bloques de datos de los directorios partiendo desde el directorio raiz (que siempre sabemos su bloque inicial) y cuando nos vamos encontrando con las dir entries que estamos buscando recorremos los bloques de datos del directorio encontrado.

Se puede implementar como una funcion recursiva que recibe la direccion del bloque de datos inicial del directorio y/o el struct `fat_dir_entry`. Arrancas en `root` buscando el primer directorio, cuando lo encontras llamas recursivamente a la funcion buscando el siguiente directorio y asi sucesivamente hasta que encuentres el dir entry del archivo que estabas buscando. 

Lo que tiene de bueno para implementar esta funcion es que como los directory entries en fat son de tamaño fijo entonces nunca nos va a quedar una `dir_entry` entre dos bloques por lo que podemos hacer la recorrida leyendo de a un bloque a la vez. Tambien la forma de conseguir los siguientes bloques de datos es mas facil que en `ext2` porque simplemente hay que preguntarse en la tabla cual es el siguiente bloque de datos del directorio actual, en cambio en `ext2` hay que embarrarse con las indirecciones. 

Para implementar `inode_from_path` en `ext2` la idea es similar pero es mas dificil de implementar por la longitud variable de los archivos. 

La idea es partiendo del inodo distinguido de `root` ir leyendo sus bloques de datos e interpretadolos como `dir_entries`. Una vez que encontramos la `dir_entry` que forma parte del path podemos obtener su inodo y volver a realizar el mismo recorrido sobre el inodo del directorio actual hasta encontrarse el inodo hoja de la ruta. 

Para ir leyendo sus bloques de datos no es tan trivial como en la fat que simplemente nos preguntamos cual es el siguiente bloque de datos en la tabla si no que necesitamos meternos con las indirecciones. Para esto habria que codear una funcion auxiliar que nos devuelva para un inodo su `i_esimo` bloque de datos, si esta dentro de los directos simplemente devolvemos eso, pero si esta en las indirecciones habria que cargar los bloques de datos de las indirecciones y sacar de ahi la direccion del `i-esimo` bloque. 

Para combatir con la longitud variable de las `dir_entries` porque pueden tener nombres no acotados lo que deberiamos hacer es ir leyendo de a dos bloques las dir entries para que no pueda pasar que un dir entry queda entre medio de dos bloques. 