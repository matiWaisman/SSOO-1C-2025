De las siguientes estrategias de ataque a una funcion con problemas de seguridad en un sistema con `stack randomization`: 

- "Escribir el valor de retorno de una función utilizando un buffer overflow sobre un buffer en stack dentro de dicha función."
  Escribir el valor de retorno de una funcion se va a seguir podiendo hacer, lo que mitiga `stack randomization` es que esa direccion virtual a la que saltamos no es realmente a la que queremos. Pero aun asi se puede pisar la direccion de retorno y se puede romper el flujo de ejecucion del programa. Este ataque se ve afectado por el `stack randomization` pero aun asi se puede seguir haciendo. 

- "Utilizar el control del valor de retorno de una función para saltar a código externo introducido en un buffer en stack controlado por el usuario."

  Este ataque si es mitigado por `stack randomization`. Porque como se cambian las direcciones virtuales de cada funcion no se puede determinar a donde arranca y donde termina uno. Por lo tanto por mas que podamos pisar el valor de retorno de una funcion no vamos a poder ejecutar otra parte del programa porque no conocemos su direccion virtual. 

- "Utilizar el control del valor de retorno de una función para ejecutar una syscall particular (por ejemplo read) que fue usada en otra parte del programa."
  Como no conocemos las direcciones virtuales no podemos determinar donde fue ejecutada esa syscall. Asi que este ataque tambien es mitigado. 