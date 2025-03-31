#include <stdio.h>
#include <stdlib.h>

// Asumo que si o si van a ingresar 11 digitos numericos
int main(){
    __uint8_t* array_numeros = malloc(11 * sizeof(__uint8_t));
    __uint8_t array_coeficientes[] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    char ch;
    printf("Ingresa tu cuit: ");
    __uint32_t i = 0;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        array_numeros[i] = ch - 48;
        i++;
    }
    __uint16_t acumulador_suma = 0;
    for(__uint32_t i = 0; i < 9; i++){
        acumulador_suma += array_numeros[i] * array_coeficientes[i];
    }
    __uint8_t suma_mod_11 = acumulador_suma % 11;

    if(11 - suma_mod_11 == array_numeros[10]){
        printf("El CUIT ingresado es valido");
    }
    return 0;
}