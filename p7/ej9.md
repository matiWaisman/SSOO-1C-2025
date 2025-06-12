Un `int` tiene 32 bits. `bits_per_page` nos dice el tamaño de cada pagina. Cada pagina va a medir $2^{bits_per_page}$ bits. 

Con la máscara `(-1) << bits_per_page`, lo que hacemos es poner ceros en los `bits_per_page` bits menos significativos y unos en los `32 - bits_per_page` bits mas significativos. Eso si es positivo, pero si es negativo. 

Primero le paso una dirección para que `addr_in_page(paddr, b, addr)` de falso. Si le paso que b es igual a `sizeof(int) - 1` la mascara va a ser de un 1 en el bit más significativo y ceros en todos los demás bits. 

Entonces el segundo llamado la mascara va a quedar con todos ceros. 