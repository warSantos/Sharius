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

// Ajuda para esclarecimento de envio de mensagens, contém
// exemplos de todas as opções de envio e comandos extras.
void ajudaMenssagem();

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
            
            // Se o último comando utilizado nao foi o de alterar para o modo de comando...
            if(buffer[0] != '!' && buffer[1] != 'c') {
                
                printf("\n\nUtilizei um comando.\n\n");
            }
            
        }else{ // Modo de envio de menssagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!' && buffer[1] != 'c') {
                
                /*
                if(strncmp(bu)){
                
                }else{
                    
                    if(listaVazia(listaLogin)) {

                        printf("\n\nSua sala esta vazia.\nAdicione pessoas a sala.\n\n");
                    } else {

                        printf("\n\nEnviei uma mensagem.\n\n");
                    }
                }
                */
                ajudaMenssagem();
            }
        }
    }
    free(buffer);
    free(alteraModo);
    return 0;
}

void ajudaMenssagem(){
    
    printf("\n\tAjuda.\n\n");
    printf("\n\tPor padrão ao se conectar a uma sala suas mensagens são enviadas");
    printf("\n\tpara todos usuários onlines na sala.");
    printf("\n\n\tPara enviar mensagens em particular adicione @ antes do nick\n");
    printf("\tdo receptor.");
    printf("\n\n\t> @FULANO MENSSAGEM...\n\n");
    printf("\t> !c altera para o modo de comando.\n\n");    
}