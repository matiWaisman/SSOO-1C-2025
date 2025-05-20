## ¿Cuáles de las siguientes opciones describen el concepto de driver?

- Es una pieza de software
  - Si. Los drivers son programas que usa el sistema operativo para manejar/ comunicarse con los controladores de los dispositivos de Entrada Salida. 

- Es una pieza de hardware
  - No. Si es Software no es Hardware.

- Es parte del SO.
  - Depende del sistema/ interpretación. En Unix esto es cierto, porque los drivers funcionan como modulos que estan cargados al sistema operativo los cuales corren en nivel de privilegio cero. Por más que hayan modulos que no vegan con la instalación Linux permite cargar/ descargar modulos dinamicamente. 

- Dado que el usuario puede cambiarlo, es una aplicación de usuario.
  - Los drivers no son una aplicación de usuario porque corren en nivel de privilegio cero dentro del kernel, no estan diseñados para ser un programa el cual interactua con el usuario.

- Es un gestor de interrupciones.
  - Los drivers no son los que atienden las interrupciones, las interrupciones las atiende el kernel. Pero los drivers pueden proveer rutinas de atención de interrupciones. 

- Tiene conocimiento del dispositivo que controla pero no del SO en el que corre.
  - Si tiene conocimiento del sistema en el que corre. Porque al tener que hacer syscalls necesita saber cuales usar en su codigo. No es lo mismo la syscall en Windows que en Linux. 

- Tiene conocimiento del SO en el que corre y del tipo de dispositivo que controla, pero no de
las particularidades del modelo específico.
  - Depende de a que se refiere el enunciado con particularidades. En la gran mayoría de los casos los drivers si conocen las particularidades del dispositivo que controla. 
