/*
 *
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 * 
 */

#include <stdio_ext.h>
#include "../headers/usuario.h"

/*
 * Lista de comandos.
 *      
 *      add (adiciona usuário).
 *      remove (remove usuário).
 *      list (lista usuários).
 *      send (envia mensagem para broadcast) OBS* modo padrão.
 *      quit (sai do server)
 *      help (Mostra comandos disponíveis).
 */

int main(void){
    
    Descritor *listaLogin = iniciarLista();
    char *buffer = calloc(sizeof(char),251);
    char *alteraModo = calloc(sizeof(char),3);
    alteraModo[0] = '!';
    alteraModo[1] = 'c';
    printf("\n\n\tCarregando configurações...\n\n");
    // Menu de comandos.
    while(1){
        
        if(alteraModo[0] == '!' && alteraModo[1] == 'c'){
            
            printf("$ ");
        }else{
            
            printf("> ");
        }
        
        scanf("%250[^\n]s", buffer);
        __fpurge(stdin);
                        
        // Identifica o princípio de um possível comando de alteração de modo.
        if(buffer[0] == '!'){ 
            
            // Altera pro modo de comando.
            if(buffer[1] == 'c'){
                
                system("clear");
                printf("\n\nModo de comando.\n\n");
                alteraModo[0] = '!';
                alteraModo[1] = 'c';
            // Altera para o modo de mensagem.    
            }else if(buffer[1] == 'm'){
                
                system("clear");
                printf("\n\nModo de mensagem.\n\n");
                alteraModo[0] = '!';
                alteraModo[1] = 'm';
            }
        }
        
        // Se estiver no mode de comandos...
        if(alteraModo[0] == '!' && alteraModo[1] == 'c'){ // chama o menu de comados.
            
            printf("\n\nUtilizei um comando.\n\n");
        }else{ // Modo de envio de menssagem...
            
            if(listaVazia(listaLogin)){
                
                printf("\n\nSua sala esta vazia.\nAdicione pessoas a sala.\n\n");
            }else{
                
                printf("\n\nEnviei uma mensagem.\n\n");
            }            
        }
    }
    free(buffer);
    free(alteraModo);
    return 0;
}
