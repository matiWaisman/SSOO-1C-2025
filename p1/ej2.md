```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pcb.h"

// pcb_0 es el desalojado y el pcb_1 el que va a ser alojado
Ke_context_switch(PCB* pcb_0, PCB* pcb_1){
    // Guardar los registros que se metieron en la pila y guardarlos en pcb_0. Deberia haber una funcion que haga esto pero el ejercicio no dice nada
    // Actualizar el PC en pcb_0 por el que se pusheo en la pila
    pcb_0->CPU_TIME = ke_current_user_time(); // devuelve el valor del cronómetro
    pcb_1->CPU_TIME = 0; 
    pcb_0->STAT = KE_READY;
    pcb_1->STAT = KE_RUNNING;
    ke_reset_current_user_time(); // resetea el cronómetro
    set_current_process(pcb_1->P_ID); // asigna al proceso con el pid como el siguiente a ejecutarse, entiendo que tambien carga el valor del ip al del nuevo proceso y tambien los registros
    ret(); // desapila el tope de la pila y reemplaza el PC
}
```c