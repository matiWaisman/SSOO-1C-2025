## Punto A 

Concentrandome unicamente en la parte del stack en donde se guardan las variables locales. 

Como primero se declara `realpass` y despues el struct, y el stack crece de arriba hacia abajo, entonces en memoria se va a ver como: 

<pre>
Direcciones de memoria (crece hacia arriba)
↑   Dirección más alta 
│   [ realpass[24-32] ]  <span style="color:red"><-- Con el segundo fgets tiene control hasta acá</span>
│   [ realpass[16-23] ]           
│   [ realpass[8-15] ]
│   [ realpass[0-7] ]
│   [ user.pass[24-32] ]  <span style="color:blue"><-- Con el primer fgets tiene control hasta acá</span>
│   [ user.pass[16-23] ]           
│   [ user.pass[8-15] ]
│   [ user.pass[0-7] ] <span style="color:red"><-- Con el segundo fgets el usuario tiene control desde acá</span>
│   [ user.name[24-32] ]  
│   [ user.name[16-23] ]           
│   [ user.name[8-15] ]
│   [ user.name[0-7] ] <span style="color:blue"><-- Con el primer fgets el usuario tiene control desde acá</span>
↓   Dirección más baja
</pre>

Al final el usuario va a tener acceso a toda la parte de variables locales del stack porque cada `fgets` le da acceso a dos variables. 

## Punto B
Ingresando como nombre el usuario correcto y para los siguientes 32 bits una cadena arbitraria. 

Si cuando nos piden la password ponemos esa cadena arbitraria podemos ingresar.