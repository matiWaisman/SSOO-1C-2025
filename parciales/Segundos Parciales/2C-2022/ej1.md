Es el 10 de la guia 6, copio y pego :P

`struct_entrada_directorio` deberia tener un nombre, de tamaño fijo, la direccion del bloque inicial y el tamaño del archivo. 

```c
struct entrada_directorio {
        char name[256];
        int attr;
        int initial_block_address;
}
```

```c
void cargar_archivo(char ** directorios){
    int* raw_data = root_table(); // Obtengo el puntero al inicio de los bloques del directorio root
    for(int i = 0; i < sizeOf(directorios); i++){
        struct entrada_directorio * entradas_dir_actual =  parse_directory_entries(raw_data);
        for(int j = 0; j < sizeOf(entradas_dir_actual); j++){
            if(entradas_dir_actual[j] -> name == directorios[i]){
                int cantidad_bloques_directorio = contar_cantidad_bloques(entradas_dir_actual[j]->initial_block_address);
                int * direccion_bloques = make_blocks(entradas_dir_actual[j]->initial_block_address, cantidad_bloques_directorio);
                raw_data = read_blocks(direccion_bloques);
                free(direccion_bloques);
                if(i == sizeOf(directorios) - 1){
                    return raw_data;
                }
                else{
                    break;
                }
            }
        }
    } 
}
```
```c
int contar_cantidad_bloques(int initial_block_address){
    // Supongo que tengo acceso a la fat para poder hacer FAT[i]
    int current_block_address = initial_block_address;
    int res = 0;
    while(current_block_address != -1){ // Supongo que el eof es -1
        res++;
        current_block_address = FAT[current_block_address];
    }
    return res;
}

// El caller le tiene que hacer free
int * make_blocks(int initial_block_address, int cantidad_de_bloques){
    int * res = malloc(cantidad_de_bloques);
    int * puntero = res;
    for(int i = 0; i < cantidad_de_bloques; i++){
        res[i] = puntero;
        puntero++;
    }
    return res;
}
```