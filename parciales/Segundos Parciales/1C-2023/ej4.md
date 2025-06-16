## Punto A

La vulnerabilidad del código está en el momento que hace `strcpy`. Como `strcpy` copia el contenido del buffer `str` a `buffer` hasta encontrarse con un null terminating caracter en `str`. Como no chequea restricciones de tamaños podemos hacerle un overflow al buffer y escribir en el stack de la función `copiar_en_minuscula` y saltar a `acceder_al_sistema` por más que no exista/ coincida nuestra contraseña. 

Suponiendo que el stack tiene la pinta: 

Direcciones de memoria (crece hacia arriba)
↑   Dirección más alta 
│   [ Parámetros de la función ]
│   [ Dirección de retorno ]
│   [ ebp ]
│   [ buf[8-15] ]  
│   [ buf[0-7] ] 
↓   Dirección más baja

Para poder saltar a `acceder_al_sistema` habría que al ejecutar el programa pasarle los primeros 16 bytes con cualquier cosa, despúes 4 bytes para pisar el ebp y la direccion para ir a acceder al sistema que es `0x555555555220` para pisar la dirección de retorno de `copiar_en_minuscula`.  

## Punto B

Para solucionar la vulnerabilidad por el lado del código habría que en vez de usar `strcpy` usar algo como `sprintf` para poder copiar pero respetando la cantidad de caracteres que vamos a copiar de `str` a `buffer`. Habría que poner: `sprintf(buffer, "%16s", str)` O si no `strncpy(buffer, str, 16)` para que no se pueda producir un buffer overflow. 

Los mecanismos del sistema operativo para contrarrestar el ataque sin que haga falta modificar el codigo es o un canario en el stack, que al darse cuenta que se modifico su posicion de memoria en el stack mate al proceso. O hacer `stack randomization` para no poder saltar a `acceder_al_sistema` porque la direccion de memoria virtual en la que se encuentra es random. Aunque aún así se podría romper la ejecución del programa en ambas, porque con el primer mecanismo se mata al proceso y con el segundo se va a saltar a una dirección de memoria que puede romper el flujo de ejecución del programa. 