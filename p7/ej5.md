## Punto A 
El mecanismo que le permite a un usuario normal tener acceso momentáneo a `/etc/shadow` es la función `setuid`. 

Los procesos tienen dos `uid`: el real, que es el del usuario que inició el proceso, y el efectivo, que indica qué permisos tiene actualmente el proceso y puede irse modificando.

`setuid` puede cambiar el `effective id`, haciendo que el owner de este proceso sea `root` para poder tener acceso a `/etc/shadow`.

El sistema le puede dar momentáneamente acceso a `/etc/shadow` y, una vez que se termina de hacer el proceso del `login`, se le degradan los permisos al proceso, haciendo que pierda los privilegios de `root`. 

## Punto B
La función tiene problemas de seguridad por nuestro queridísimo `gets`. Como `gets` no deja de leer hasta que se ingrese un `\n` o un `EOF`, podemos producir un `buffer overflow`. También es un problema el orden en el que se carga la verdadera contraseña y la contraseña ingresada. 

Como la primera variable que se declara es `pass`, si ingresamos un string de 384 bytes vamos a poder modificar `realpass` y `salt`. 

Si se invirtiera el orden en el que se pide input y se carga la contra y el salt, ahí no podríamos sobreescribir `realpass` y `salt`, pero sí podríamos modificar la dirección de retorno del programa. 

El usuario va a poder controlar la dirección de retorno y todas las variables locales de la función. 

## Punto C
Si uno bloquea la pantalla y se va al baño, alguien podría robarse el dispositivo y aprovecharse de este exploit para acceder a tus datos. 

También es un problema de uso. Si alguien por accidente escribe demasiadas letras, porque por ejemplo apoyó algo sobre el teclado, podría tocar Enter porque total es más fácil que te rechace la contraseña que borrar todo. Pero como pisamos la dirección de retorno, se va a romper el login, causando un problema de disponibilidad. 
