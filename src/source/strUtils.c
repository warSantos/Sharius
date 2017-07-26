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

Comando *split(char *buffer){
    
    Comando *bloco = calloc(sizeof(Comando),1);
    
    // Procurando por espaço "lixo antes de digitar o comando.
    int i = 0, j;
    while(buffer[i] == ' '){ ++i;}
    
    j = 0;
    bloco->comando = malloc(sizeof(char) * 7);
    
    // Copiando o comando do buffer para a string de comando.
    while(buffer[i + j] != ' ' && buffer[i + j] != '\0'){
        
        bloco->comando[j] = buffer[i + j];
        ++j;
    }
    // Atribuindo tamanho do comando.
    bloco->comando[j + 1] = '\0';
    bloco->lenghtComando = j;
    i += j;
    bloco->parametro = calloc(sizeof(char), 243);
    // Detecta a inexistência de parâmetro.
    if(buffer[i] == '\0'){
        
        bloco->parametro[0] = '!';
        bloco->parametro[1] = '\0';
        bloco->lenghtParametro = 2;
    }else{
        j = 0;
        while(buffer[i] != '\0'){
            
            bloco->parametro[j] = buffer[i];
            ++i;
            ++j;
        }
        // Atribuindo tamanho do parâmetro.
        bloco->lenghtParametro = j;
        bloco->parametro[j] = '\0';
    }
    return bloco;
}