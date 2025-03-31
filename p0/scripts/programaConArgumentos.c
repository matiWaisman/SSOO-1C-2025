#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    int numero;
    int cantidad_letras_texto;
    printf("Ingresa una cadena de texto: ");
    char *cadena = NULL;
    size_t size = 0;
    getline(&cadena, &size, stdin);
    printf("Ingresa un numero: ");
    scanf("%d", &numero);
    
    if(numero < strlen(cadena)){
        char *string_reverso = malloc(strlen(cadena) + 1);
        __uint32_t j = 0;
        for(__uint32_t i = strlen(cadena); i > 0; i--){
            string_reverso[j] = cadena[i];
            j++;
        }
        string_reverso[j] = '\0';
        printf("La cadena invertida es: %s", cadena);
        free(string_reverso);
    }
    else{
        printf("No hace falta invertir la cadena");
    }
    free(cadena);
    return 0;
}