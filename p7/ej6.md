## Punto A
Para que se ejecute el `assert` no se tiene que cumplir ninguna de las tres condiciones. 

Como en el estándar `IEEE-754` existe el `Not a Number` que representa resultados indefinidos, como la división por cero o la raíz cuadrada de un número negativo.

Entonces a la función `signo` le podemos pasar un `NaN` para que no entre en ningún if. Porque cualquier comparación contra el `NaN` numérica da falso. 

## Punto B
Depende qué se haga con el resultado de la función `signo`. Pero devolvería el número cero en vez del 1, 2 o 3. 

Si no se handlea el caso en el que el resultado es cero podría causar problemas. 

También si un usuario supiera cómo es el flujo de ejecución de la aplicación podría alterarlo. 
