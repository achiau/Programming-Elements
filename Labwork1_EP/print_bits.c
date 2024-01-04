/*************************************************************************************************************************************************
Script  name: ex2_labw1.c                                                                                                                       
Project name: Print_Bits                                                                                                                        
Usage:./ex2_labw1

Description: This program reads an integer, i, from the keyboard  prints it in binary representation of i      
              from the most siginmificant bit
Bugs: none
Author: Agostinho Chiau 
**************************************************************************************************************************************************/

#include <stdio.h>
 
 
    // Declaração de variáveis
    int i;
 //Declaraçao da Função para imprimir um  interiro em representaçao binária
void printBinary(int num){
	int bit;
	
	//Determina o número de bits num interiro
	int numBits = sizeof(int)*8;
	
        // Iteração do bit mais até  menos significativo
    for (int i = numBits - 1; i >= 0; i--) {
        // Extrai o valor do bit da  posição i, e imprime-o
        bit = (num >> i) & 1;
        printf("%d", bit);

        // Adiciona espaço após cada 4 bits  para uma melhor leitura 
        if (i % 4 == 0) {
            printf(" ");
        }
    }

    printf("\n");
}

// Inicialização
int main() 
{


    // Leitura do número inteiro i 
    printf("Enter an integer: ");
    scanf("%d", &i);

    // Imprimi o inteiro em representação binária de i
    printf("Binary representation:");
    printBinary(i);

    return 0;
}
 

 
