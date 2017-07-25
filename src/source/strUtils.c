#include "../headers/strUtils.h"

int charInvalido(char *nick, char *caracteresInvalidos){
    
    int i,j;
    for(i = 0; nick[i] != '\0'; ++i){
        
        for(j = 0; caracteresInvalidos[j] != '\0'; ++j){
            
            if(nick[i] == caracteresInvalidos[j]){
                
                return 1;
            }
        }
    }
    return 0;
}

int verificaIp(char *ip){

    int i, qtdePontos = 0;    
    for(i = 0; ip[i] != '\0'; ++i){
        
        if(ip[i] == '.'){
            
            ++qtdePontos;
            if(i == 0){
                
                printf("IP inválido.\nPrimeiro octeto vazio!\n");
                return 1;
            }else if(ip[i + 1] == '\0'){
                
                if(qtdePontos <= 3){
                    
                    printf("Endereço imcompleto.\n");
                    return 1;
                }else{
                    
                    printf("Endereço com octetos a mais.\n");
                    return 1;
                }                
            }
            
        }else if(ip[i] < 49 || ip[i] > 57){
            
            printf("Caracter %c inválido, posição %d.\n", ip[i], i);
            return 1;
        } 
    }
    return 0;
}

