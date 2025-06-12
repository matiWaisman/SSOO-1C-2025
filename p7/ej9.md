Un `int` tiene 32 bits. `bits_per_page` nos dice el tamaño de cada pagina. Cada pagina va a medir $2^{bits_per_page}$ bits. 

Con la máscara `(-1) << bits_per_page`, lo que hacemos es poner ceros en los `bits_per_page` bits menos significativos y unos en los `32 - bits_per_page` bits mas significativos. Eso si es positivo, pero si es negativo 