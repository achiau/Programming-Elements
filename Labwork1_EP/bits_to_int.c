/*************************************************************************************************************************************************
Script  name: ex3_labw1.c                                                                                                                       
Project name: Bits_to_Int                                                                                                                       
Usage:./ex3_labw1

Description: This program reads an integer, i, from the keyboard  prints it in binary representation of i      
              from the most siginmificant bit
Bugs: none
Author: Agostinho Chiau 
**************************************************************************************************************************************************/

#include <stdio.h>
#include <string.h>


// Funçao para converter um número Binário em inteiro
int binaryToInt(char binary[]) {
    int decimal = 0;
    int length = strlen(binary);

    // Iteração  sobre os caracteres da string binária
    for (int i = 0; i < length; i++) {
        // Conversão de caracter para inteiro ('0' -> 0, '1' -> 1)
        int bit = binary[i] - '0';

        // Atualizar o valor decimal  baseando se o bit atual
        decimal = (decimal << 1) | bit;
    }

    return decimal;
}

// Inicialização
int main() {
    // Declaração do array para armazenar a string binária
    char binary[64]; // Ajustável consoante a necessidade

    // Lê a string binária do teclado
    printf("Introduza a string binária: ");
    scanf("%s", binary);

    // Converte a String binária em inteiro
    int decimalValue = binaryToInt(binary);

    // Imprime o valor inteiro decimal correspodente
    printf("Representação Decimal: %d\n", decimalValue);

    return 0;
}
