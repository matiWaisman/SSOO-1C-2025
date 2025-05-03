## Código A

Siendo que para realizar la operación `x = x + 1`, al traducir la línea de código a *assembler* se separa en los pasos:

- Cargar el valor de X desde memoria a un registro (*Load*)
- Sumarle uno al registro
- Guardar el valor del registro en memoria (*Store*)

El *scheduler* nos puede interrumpir durante una de las tres operaciones.

Por lo tanto, puede pasar que:

- Se ejecute primero **todo** el Proceso A y luego **todo** el Proceso B, por lo que en consola se va a imprimir `1`.
- Se ejecute primero **todo** el Proceso B y luego **todo** el Proceso A, por lo que en consola se va a imprimir `2`.
- Se ejecute la operación de lectura en el Proceso A, pero antes de que vuelva a guardar el valor de X actualizado, B lea X y este siga valiendo cero, por lo que después le va a sumar también uno a cero, haciendo que ambos procesos terminen guardando un `1` en la variable X.
- Se ejecute la operación de lectura en el Proceso B, pero antes de que vuelva a guardar el valor de X actualizado, A lea X y este siga valiendo cero, por lo que después le va a sumar también uno a cero, haciendo que ambos procesos terminen guardando un `1` en la variable X.

## Código B

Lo "esperado" podría ser, si el *scheduler* es *round robin* súper justo para este programa, que haya exactamente 4 *prints* del valor de X y exactamente 4 *prints* que digan `"a"`. Otra opción podría ser que se ejecute primero todo el Proceso A, haciendo que B no se ejecute nunca.

Pero por el orden en el que ejecuta el *scheduler* el código, puede pasar que haya más de 4 *prints* de `"a"` o menos. Pero sí o sí van a estar los 4 *prints* de los valores de X.

Para que haya más de 4 *prints* de `"a"`, es porque el *scheduler* interrumpe al Proceso A después de guardar 1 en la variable `y` y se ejecuta el Proceso B varios ciclos.

Para que haya menos de 4 *prints* de `"a"`, puede ser porque el *scheduler* le da más tiempo al Proceso A antes de que se ejecute el B y ya X arranque con un valor mayor a `0` y después se repartan el tiempo de ejecución equitativamente.

También el orden de los *prints* no es determinístico, no se puede asegurar si primero se va a imprimir el valor de X o una `"a"`.

