Para desarrollar el software del autito vamos a tener dos componentes repetidos. El primero son los tres sensores, uno en el medio y uno en cada costado. El segundo componente va a ser el motor, que tenemos uno por cada rueda. 

Va a ser necesario implementar solamente dos drivers, uno para el manejo individual de cada rueda y otro para el manejo individual de cada sensor. 

El que realmente se va a encargar de hacer que el autito siga la trayectoria deseada va a ser el codigo de usuario, pero por medio de los drivers nos vamos a poder comunicar con el hardware. 

El sensor va a tener un unico registro de solo lectura `IS_STANDING_ON_BLACK` que va a valer uno si lo que esta viendo es de color negro y cero si no. 

El motor de las ruedas va a tener un registro de escritura `CURRENT_PACE` que si vale cero significa que el motor esta apagado y entre mas alto sea este valor mas rapido va a ir la rueda. 

Escribo primero el codigo de usuario que va a manejar el auto: 

```c
void main(int argc, char ** argv){
    // Los tres sensores son manejados por el mismo driver
    int sensor_izq = open("/dev/sensor_izq"); 
    int sensor_medio = open("/dev/sensor_medio");
    int sensor_der = open("/dev/sensor_der");
    // Las dos ruedas son manejadas por el mismo driver
    int rueda_izq = open("/dev/rueda_izq");
    int rueda_der = open("/dev/rueda_der");
    // Seteo para que arranque moviendose al medio suponiendo que siempre va a arrancar sobre negro en el medio 
    write(rueda_izq, 25);
    write(rueda_der, 25);
    while(1){
        // Si leo que se desvio lo arreglo hasta que se vuelva a centrar 
        if(read(sensor_izq) == 1){
            write(rueda_izq, 50); // Le duplico la velocidad para que gire a la derecha. 
            while(read(sensor_medio) != 1){} // Hago busy waiting
            // Una vez que estoy aca abajo es porque se enderezo la direccion asi que le bajo la velocidad a la rueda para que deje de doblar
            write(rueda_izq, 25);
        }
        if(read(sensor_der) == 1){
            write(rueda_der, 50); // Le duplico la velocidad para que gire a la derecha. 
            while(read(sensor_medio) != 1){} // Hago busy waiting
            // Una vez que estoy aca abajo es porque se enderezo la direccion asi que le bajo la velocidad a la rueda para que deje de doblar
            write(rueda_der, 25);
        }
        
    }
}
```





