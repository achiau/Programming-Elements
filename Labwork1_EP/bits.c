/*************************************************************************************************************************************************
Script  name: bits                                                                                                                                                                                                                                           
Usage:./bits <integer>

Description: This program its a joint of print_bits and bits_to int scripts..basicaly In this case, the needed
             inputs (integer ) is passed as a command-line arguments
             
Bugs: none
Author: Agostinho Chiau 
**************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// Função que imprime a representação binário dum inteiro  
void printBinary(int num) {
    int bit;

    // Determinaçao do numero de bits num inteiro
    int numBits = sizeof(int) * 8;

    // Iteracao do bit mais significativo para o menos  significativo
    for (int i = numBits - 1; i >= 0; i--) {
        // Extracao do valor do bit na posicao i
        bit = (num >> i) & 1;

        // Imprimi bit
        printf("%d", bit);

        // Adiciona espaço após cada 4 bits para uma melhor leitura 
        if (i % 4 == 0) {
            printf(" ");
        }
    }

    printf("\n");
}

// Inicializaçao 
int main(int argc, char *argv[]) {
    // Verifica se o numero correto do argumento da  linha de comando é fornecido
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1; // Exit with an error code
    }

    // Converte o argumento da linha de comando num inteiro 
    int i = atoi(argv[1]);

    // Imprimi a representaçao decimal do inteiro introduzido 
    printf("Decimal representation: %d\n", i);

    // Imprimi a representaçao binaria do interio iintroduzido 
    printf("Binary representation: ");
    printBinary(i);

    return 0;
}
