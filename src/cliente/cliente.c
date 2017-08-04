#include "cliente.h"
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include<unistd.h>    //write

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void ajudaMensagem(){
        
    printf("\n\tAjuda.\n\n");
    printf("\n\t!c altera para o modo de comando.\n\n");    
    printf("\n\tPor padrão ao se conectar a uma sala suas mensagens são enviadas");
    printf("\n\tpara todos usuários onlines na sala.");
    printf("\n\n\tPara enviar mensagens em particular adicione @ antes do nick\n");
    printf("\tdo receptor.");
    printf("\n\n\t> @FULANO MENSSAGEM...\n\n");    
    printf("\tTODA mesagem iniciada por @ será comprendido como uma mensagem privada.\n");
    printf("\tEntão evite iniciar mesagens com @ com destino para broadcast (todos na sala).\n");
    printf("\n\t-clear - limpa o bufferscreen (tela).");
    printf("\n\t-list - imprimi lista de usuários ativos.");    
}

void ajudaComando(){

    printf("\n\tAjuda.\n\n");
    printf("\n\t!m altera para o modo de mensagem.\n\n");        
    printf("\n\tclear - limpa o bufferscreen (tela).");
    printf("\n\tlist - imprimi lista de usuários ativos.");
    printf("\n\thelp - consulta ajuda.");
    printf("\n\tquit - encerra conexão com logins e deleta sala.");        
}

int menuComando(char *buffer){
    
    Comando *bloco = split(buffer);
    if(!bloco->lenghtComando){
        
        return 1;
    }
    if(!strncmp(bloco->comando, "list", 5)){
        
        imprimirLista(listaLogin);
    }else if(!strncmp(bloco->comando, "quit", 5)){
        
        printf("Fechando conexões...\n");
        printf("Have a nice day....\n");
        return 0;
    }else if(!strncmp(bloco->comando, "help", 5)){
        
        ajudaComando();
    }else if(!strncmp(bloco->comando, "clear", 6)){
        
        system("clear");
    }else{
        
        printf("\nComando não identificado.\n\n");
    }
    return 1;
}

void menuMensagem(char *buffer, char *userNick, int idSocket){
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMensagem(buffer);    
    if(!bloco->lenghtParametro){
        
        return;
    }
    //printf("valor de retorno %d\n", strncmp(bloco->parametro, "-help", 6));
    if(!strncmp(bloco->parametro, "-help", 6)){
        
        ajudaMensagem();
        return;
    }else if(!strncmp(bloco->parametro, "-clear", 7)){
        
        system("clear");
        return;
    }else if(!strncmp(bloco->parametro, "-list", 6)){
        
        imprimirLista(listaLogin);
        return;
    }     
    enviarBloco(buffer, userNick, idSocket);    
}

void menuOperacao(char *userNick, int idSocket){
    
    listaLogin = iniciarLista();
    char *buffer;
    char *alteraModo = calloc(sizeof(char),3);
    alteraModo[0] = '!';
    alteraModo[1] = 'c';
    printf("\n\n\tCarregando configurações...\n\n");
    // Menu de comandos.
    int loop = 1;
    while(loop){
        
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
            
            system("clear");
            // Altera pro modo de comando.            
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
            if(buffer[0] != '!' || buffer[1] != 'c') {
                                
                if(!menuComando(buffer)){
                    
                    exit(0);
                }                
            }                        
        }else{ // Modo de envio de mesagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!'|| buffer[1] != 'm') {
                
                menuMensagem(buffer, userNick, idSocket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

void recebeMensagem(void *socketServer){
    
    //Id de ientificação do cliente que utiliza a função
    // Podem ser várias threads desta liberadas
    // Logo o idSocket de cada chamada da função escutaCliente 
    // é diferente é um socket diferente.
    printf("Servidor de mensagens pronto...\n\n");
    
    int read_size;    
    int idSocket = *(int*) socketServer;
    char *buffer, *userNick;
    
    // recebe mensagens do cliente.
    while((read_size = recebeBloco(&buffer, &userNick, idSocket)) > 0){
                        
        // extraindo mensagem do buffer recebido.
        Comando *bloco = extraiMensagem(buffer);        
        printf(ANSI_COLOR_RED "@%s -:> %s" ANSI_COLOR_RESET "\n",userNick, bloco->parametro);       
    }            
    if(read_size == 0){
        
        puts("Client disconnected");
        fflush(stdout);        
    }else if(read_size == -1){
        
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socketServer);
}