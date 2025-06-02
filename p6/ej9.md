Si un proceso abre el directorio `/home` y le envía el descriptor a otro, suponiendo que con el descriptor podemos acceder al `inodo`, como una vez dentro del `inodo` de `/home` no sabemos fácilmente el nombre del archivo, ni dónde estamos parados dentro de la jerarquía del `file system`.

Para que el otro proceso pueda conseguir el nombre del inodo (`home`) lo que habría que hacer es: 

- Buscar el número de `inodo` con del `fd` que nos pasan. 
- A partir de la función `load_inode`, cargamos el inodo en memoria usando el número que conseguimos del `fd`.
- Nos guardamos el número de inodo de `.`. 
- Cargamos en memoria el `inodo` del directorio padre usando el hard link de `..`. 
- Recorremos los bloques de datos del inodo padre hasta encontrar que el inodo del dir entry es igual al que conseguimos en `.`. 
- Hacemos una copia del nombre de esa dir entry. 
- Devolvemos la copia.
