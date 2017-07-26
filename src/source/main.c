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

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
void menuComando(char *buffer);

// Função para delegar o buffer a função de estraiMenssagem para processamento
// e delegação dos dados processados para a função de enviar menssagem.
void menuMenssagem(char *buffer);

int main(void){
    
    listaLogin = iniciarLista();
    char *buffer;// = calloc(sizeof(char),251);
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
        buffer = calloc(sizeof(char),251);
        scanf("%250[^\n]s", buffer);
        __fpurge(stdin);
                        
        // Identifica o princípio de um possível comando de alteração de modo.
        if (buffer[0] == '!' && buffer[1] == 'c') {
            
            // Altera pro modo de comando.
            system("clear");
            printf("\n\nModo de comando.\n\n");
            alteraModo[0] = '!';
            alteraModo[1] = 'c';
            // Altera para o modo de mensagem.    
        } else if (buffer[0] == '!' && buffer[1] == 'm') {

            system("clear");
            printf("\n\nModo de mensagem.\n\n");
            alteraModo[0] = '!';
            alteraModo[1] = 'm';
        }        
        
        // Se estiver no mode de comandos...
        if(alteraModo[0] == '!' && alteraModo[1] == 'c'){ // chama o menu de comados.
            
            // Se o último comando utilizado nao foi o de alterar para o modo de comando...
            if(buffer[0] != '!') {
                
                menuComando(buffer);
            }            
            
        }else{ // Modo de envio de menssagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!') {
                
                menuMenssagem(buffer);
                /*
                if(strncmp()){
                
                }else{
                    
                    if(listaVazia(listaLogin)) {

                        printf("\n\nSua sala esta vazia.\nAdicione pessoas a sala.\n\n");
                    } else {

                        printf("\n\nEnviei uma mensagem.\n\n");
                    }
                } 
                 */                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);
    return 0;
}

void ajudaMenssagem(){
        
    printf("\n\tAjuda.\n\n");
    printf("\n\t!c altera para o modo de comando.\n\n");    
    printf("\n\tPor padrão ao se conectar a uma sala suas mensagens são enviadas");
    printf("\n\tpara todos usuários onlines na sala.");
    printf("\n\n\tPara enviar mensagens em particular adicione @ antes do nick\n");
    printf("\tdo receptor.");
    printf("\n\n\t> @FULANO MENSSAGEM...\n\n");    
    printf("TODA menssagem iniciada por @ será comprendido como uma mensagem privada.\n");
    printf("Então evite iniciar menssagens com @ com destino para broadcast (todos na sala).\n");
}

void ajudaComando(){

    printf("\n\tAjuda.\n\n");
    printf("\n\t!m altera para o modo de mensagem.\n\n");    
    printf("\n\tadd - adiciona logins a sala.");
    printf("\n\tlist - imprimi lista de usuários ativos.");
    printf("\n\thelp - consulta ajuda.");
    printf("\n\tquit - encerra conexão com logins e deleta sala.");
    printf("\n\tremove - remove um usuário.\n\n");    
}

/*
 * Lista de comandos.
 *      
 *      add (adiciona usuário).
 *      remove (remove usuário).
 *      list (lista usuários).      
 *      quit (sai do server)
 *      help (Mostra comandos disponíveis).
 */

void menuComando(char *buffer){
    
    Comando *bloco = split(buffer);
    if(!bloco->lenghtComando){
        
        return;
    }
    if(!strncmp(bloco->comando, "add", 4)){
        
        printf("Nick add com sucesso.\n\n");
    }else if(!strncmp(bloco->comando, "remove", 7)){
        
        printf("Nick removido com sucesso.\n\n");
    }else if(!strncmp(bloco->comando, "list", 5)){
        
        imprimirLista(listaLogin);
    }else if(!strncmp(bloco->comando, "quit", 5)){
        
        printf("Fechando conexões...\n");
        printf("Have a nice day....\n");
        exit(0);
    }else if(!strncmp(bloco->comando, "help", 5)){
        
        ajudaComando();
    }else if(!strncmp(bloco->comando, "clear", 6)){
        
        system("clear");
    }else{
        
        printf("\nComando não identificado.\n\n");
    }
}

void menuMenssagem(char *buffer){
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMenssagem(buffer);
    
    printf("Usuário: %s , %d.\n", bloco->comando, bloco->lenghtComando);
    printf("-: %s , %d.\n", bloco->parametro, bloco->lenghtParametro);
}