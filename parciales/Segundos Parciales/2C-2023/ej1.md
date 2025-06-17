```c
unsigned int[] FAT;
struct FATDirEntry{
    char filename[8];
    char extension[3];
    char atribute;
    unsigned short starting_cluster; // primer bloque
    unsigned int size;
}

// Dado un path devuelve su primer bloque
unsigned int FATFS::get_first_block_from_path(const char * path);
// Se puede considerar que BLOCK_SIZE es multiplo de FAT_DIR_ENTRY_SIZE
```

```c
void mi_ls_r(char * path){
    unsigned int bloque_actual = get_first_block_from_path(path);
    char * buffer = malloc(BLOCK_SIZE);

    while(bloque_actual != -1){
        read_block(bloque_actual, buffer);
        struct FatDirEntry * puntero_dir_entries = (struct FatDirEntry *) buffer;
        int cantidad_dir_entries_en_bloque = BLOCK_SIZE / sizeof(FATDirEntry);
        // Como por la consigna el tamaño de los structs es multiplo del tamaño del bloque itero por los dir entries dentro del bloque sin preocuparme
        for(int i = 0; i < cantidad_dir_entries_en_bloque; i++){
            printf("%s", puntero_dir_entries->filename);
            if(puntero_dir_entries->attribute == 2){
                mi_ls_r(path + "/" + puntero_dir_entries->filename); // Es pseudocodigo loko, no pasa nada
            }
            puntero_dir_entries++;
        }
        // Actualizo el bloque a leer para la proxima iteracion
        bloque_actual = FAT[bloque_actual]; 
    }
    free(puntero_dir_entries);
}
```

## Punto B
En mi opinion la mayor diferencia entre un File System `FAT` versus `ext2` está en que en `FAT` es necesario tener cargada la tabla todo el tiempo en memoria, ocupando una gran cantidad de espacio en memoria principal, mientras que en un sistema basado en inodos solo es necesario tener cargado en memoria los inodos asociados a los archivos a los que queremos acceder. 

Aunque la manera de navegar por los bloques de datos de un archivo es mucho más facil de hacer en un sistema basado en `FAT` que en uno basado en inodos. En FAT lo unico que hay que hacer es recorrer los indices de los bloques todo sobre la misma tabla, en cambio en uno de inodos la recorrida no es tan líneal si no que hay que navegar a través de las indirecciones. 

Tambien la cantidad de memoria que puede ocupar un archivo es menor en inodos que en FAT. En inodos está acotada por las indirecciones la cantidad de memoria que puede ocupar un archivo, si tenemos inodos con menor cantidad de indirecciones vamos a tener un tamaño maximo más chico. En cambio con la tabla de FAT se puede asignarle a un archivo toda la memoria que pueda direccionar la tabla. 

Otra diferencia entre los File Systems es que los basados en inodos soportan hard links y symbolic links. Los links simbolicos nos permiten referenciar archivos entre File Systems distintos y discos distintos. Esto en FAT no se puede hacer. 

Hablando de las entradas de directorios en `FAT` las entradas tienen longitud fija porque el tamaño de los nombres es acotado. En cambio en un sistema basado en inodos como la longitud de los nombres no está acotada las dir entries tienen longitud variable, por lo que sumado a la recorrida por las indirecciones hace todavía más dificil recorrer dir entries en un sistema basado en inodos. 

Puede ocurrir que una entrada de directorio quede entre bloques. Eso en FAT no puede pasar.