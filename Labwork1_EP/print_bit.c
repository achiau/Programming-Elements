/*************************************************************************************************************************************************
Script  name: print_bit                                                                                                                                                                                                                                             
Usage:./print_bit

Description: This program reads an integer, i, from the keyboard and a bit position ,p, and  prints  the corresponding value 
             of that bit in the binary representation of i; (was considere p=0, which corresponds the position of the least significant bit)
Bugs: none
Author: Agostinho Chiau 
**************************************************************************************************************************************************/


#include <stdio.h>
#include <string.h>


 // Variable declaration 
    int i, p;
    
 // Initialization
int main()
{
    
    // Leitura do número inteiro i e o bit de posição p
    printf("Introduza o inteiro i: ");
    scanf("%d", &i);
 
    printf("Introduza a posição do bit p: ");
    scanf("%d", &p);
 
    // Validação do bit de posição p  
    if (p < 0 || p >= sizeof(int) * 8) {
        printf("O bit de posição é inválido. Deve estar entre 0 e %lu.\n", (sizeof(int) * 8 - 1));
        return 1; // termina o programa com código de erro 
    }
    
    // Extrair o valor do bit na posição p e imprime o resultado
    int bitvalue = (i >> p) & 1;
    printf("O bit na posição %d na representação binária de %d é: %d\n", p, i, bitvalue);
   	
    return 0;
}
