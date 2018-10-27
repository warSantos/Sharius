#include "../headers/strUtils.h"

int verificaIp(char *ip){

    int i, qtdePontos = 0, octeto = 0, cont = 1, nOcteto = 1;
    for (i = 0; ip[i] != '\0'; ++i) {
       
        if (ip[i] == '.') {
                     
            ++qtdePontos;
            if (i == 0) {

                printf("IP inválido.\nPrimeiro octeto vazio!\n");
                return 1;
            } else if (ip[i + 1] == '\0') {

                if (qtdePontos <= 3) {

                    printf("Endereço imcompleto.\n");
                    return 1;
                } else {

                    printf("Endereço com octetos a mais.\n");
                    return 1;
                }
            }
            // identificando os octetos.
            cont = 0;
        } else if (ip[i] < 48 || ip[i] > 57) {

            printf("Caracter %c inválido, posição %d.\n", ip[i], i);
            return 1;
        }                    
         
        // ja passou por um ponto.
        if (!cont) {
            
            if (!octeto || octeto > 163) {

                printf("Valor inválido no octeto: %d.\npor favor digite valores maiores que 0 e menores que 255.\n\n", nOcteto);
                return 1;
            }
            cont = 1;
            octeto = 0;
            nOcteto++;                        
        }else{
            
            octeto = octeto + ip[i];          
        }                        
    }
    if (qtdePontos != 3) {

        printf("Endereço IP inválido.\n\n");
        return 1;
    }
    return 0;
}