## Punto A
Si el hash de la contraseña es el único método de autenticación, como en el ejercicio anterior, si es relativamente fácil calcular la preimagen de un hash, es una mala idea transmitir el hash por el canal inseguro. Porque dado que conocen tu hash y tu usuario, podrían buscar qué contraseñas están asociadas a ese hash y tener acceso a tus datos.

Este esquema es vulnerable al `replay attack`. Los `replay attack` se basan en interceptar los mensajes entre un usuario y el servidor y volver a enviar ese mismo mensaje haciéndote pasar como el usuario, cuando en realidad no lo sos.

## Punto B
Si cada vez que se trata de hacer el `login` se elige una nueva seed, si un atacante intercepta tanto la seed como el hasheo de la concatenación de la seed y la contraseña, podría encontrar, por fuerza bruta, strings asociados a ese número de hash.

Pero como cada vez que hay que autenticarse se usa una seed distinta, si se quiere logear con esa string descubierta no va a poder porque la seed cambia en cada intento. El atacante siempre podría descubrir strings asociadas a la seed usada, pero como la seed cambia en cada intento de login no va a poder hacer nada con las strings encontradas.

Entonces este es un método más seguro para la autenticación sabiendo que se puede interceptar la comunicación.
