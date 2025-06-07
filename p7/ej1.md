## Punto A 
Para verificar si un usuario ingresó su contraseña correctamente, el sistema primero debería hashear la contraseña ingresada y buscar si el hash de la contraseña que ingresó matchea el hash de la contraseña del usuario guardado. 

## Punto B
Si la función de hash entrega valores de 64 bits, va a haber $2^{64}$ hashes distintos. Que, por más que sean muchísimos, los estándares como `SHA-256` tienen $2^{256}$ hashes distintos, aparte de otros mecanismos de seguridad como `salt`. 

La probabilidad de acertar al valor del hash almacenado usando una contraseña que hashee al mismo valor es de $\frac{1}{2^{64}}$, o sea 1 sobre 18.4 cuatrillones. Que en notación científica se puede escribir como $5,42 \times 10^{-20}$.

## Punto C
(No sé de probabilidad. Hay que chequear.)

Si por segundo podemos probar mil millones de contraseñas con hashes distintos: 

- En un segundo tenemos un $5,42 \times 10^{-20} \times 10^{9} \times 100$% = $5,42 \times 10^{-9}$ % de chances de acertar. 

- Como en un minuto podemos probar $60 \times 10^{9}$ contraseñas distintas. Tenemos una probabilidad de: $60 \times 5,42 \times 10^{-9}$ = $3,252 \times 10^{-7}$ % de acertar la contraseña. 

- Como en una hora podemos probar $60 \times 60 \times 10^{9}$ contraseñas distintas. Tenemos una probabilidad de: $60 \times 3,252 \times 10^{-7}$ = $1,95 \times 10^{-5}$ % de acertarle a la contraseña.

- Como en un día podemos probar $24 \times 60 \times 60 \times 10^{9}$ contraseñas distintas. Tenemos una probabilidad de: $24 \times 1,95 \times 10^{-5}$ = $4,68 \times 10^{-4}$ % de acertarle a la contraseña. 

- Como en un año podemos probar $365 \times 24 \times 60 \times 60 \times 10^{9}$ contraseñas distintas. Tenemos una probabilidad de: $365 \times 4,68 \times 10^{-4}$ = $0,17$ % de acertarle a la contraseña. 

Entonces, si por año tenemos un $0,17$% de probabilidades de acertarle a la contraseña, tendríamos que resolver la ecuación: $0,17 \times a = 50$ para encontrar la cantidad de años que habría que probar que nos garantizan tener un 50% de chances de acertarle a la contraseña. Por lo que tendríamos que estar 294-295 años para tener un 50% de probabilidad de acertar. 

## Punto D
Si la cantidad de caracteres y cuáles son está acotada. Suponiendo que se pueden escribir contraseñas con la Ñ: en total tenemos 27 caracteres, que se pueden escribir en mayúscula o minúscula, y 10 dígitos, y hay que armar contraseñas de 6 caracteres. 

Entonces hay $\binom{27 \times 2 \times 10}{6}$ = $3,35 \times 10^{13}$ contraseñas posibles. 

Como la cantidad de hashes posibles es menor que la cantidad de contraseñas totales, cada contraseña va a tener un hash único. Por lo tanto, cada contraseña que probemos va a servirnos para descartar un hash en particular. Si podemos probar $10^{9}$ contraseñas por segundo hay que resolver la ecuación: $10^{9} \times s = 3,35 \times 10^{13}$. Que da: $3,35 \times 10^{4}$. Por lo que en 9 horas y 19 minutos estamos seguros de que vamos a poder encontrar la contraseña.
