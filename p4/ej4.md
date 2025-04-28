Un `Page Fault` ocurre cuando un proceso trata de acceder a una dirección virtual que se traduce a una página que no se encuentra presente en memoria principal, porque su bit de `present` se encuentra en cero.

Para resolverlo, el sistema operativo primero ubica en la memoria secundaria la página deseada.

Una vez localizada, se verifica si existe algún frame libre en la memoria principal:

- **Si hay un frame libre**, se carga directamente desde el disco a la memoria principal la página deseada en ese lugar.
  
- **Si no hay frames libres**:
  
  - Según la política de reemplazo de páginas, se selecciona una página para eliminar de la memoria principal.
  
  - Si la página seleccionada no fue modificada (bit de `dirty` apagado), simplemente se descarta. Si fue modificada, se escribe en la memoria secundaria antes de eliminarla de la memoria principal y se actualizan las tablas de páginas acorde a este cambio.
  
  - Se carga en memoria principal la nueva página desde la memoria secundaria y se actualizan las tablas de páginas acorde a este cambio.

Y por último se sigue la ejecución del proceso que produjo el `Page Fault` o se indica que ya se puede ejecutar en el `Scheduler`.

Todas las operaciones de lectura/escritura en disco son lentas, por lo que mientras se resuelve el `Page Fault`, el proceso que lo produjo permanece bloqueado y no puede ejecutarse.  
Por este motivo, se busca siempre minimizar la cantidad de `Page Faults` para maximizar el **Throughput** del sistema.
