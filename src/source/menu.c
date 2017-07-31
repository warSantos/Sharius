#include "../headers/menu.h"
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include<unistd.h>    //write

void ajudaMenssagem(){
        
    printf("\n\tAjuda.\n\n");
    printf("\n\t!c altera para o modo de comando.\n\n");    
    printf("\n\tPor padrão ao se conectar a uma sala suas mensagens são enviadas");
    printf("\n\tpara todos usuários onlines na sala.");
    printf("\n\n\tPara enviar mensagens em particular adicione @ antes do nick\n");
    printf("\tdo receptor.");
    printf("\n\n\t> @FULANO MENSSAGEM...\n\n");    
    printf("\tTODA menssagem iniciada por @ será comprendido como uma mensagem privada.\n");
    printf("\tEntão evite iniciar menssagens com @ com destino para broadcast (todos na sala).\n");
    printf("\n\t-clear - limpa o bufferscreen (tela).");
    printf("\n\t-list - imprimi lista de usuários ativos.");    
}

void ajudaComando(){

    printf("\n\tAjuda.\n\n");
    printf("\n\t!m altera para o modo de mensagem.\n\n");    
    printf("\n\tadd - adiciona logins a sala.");
    printf("\n\tclear - limpa o bufferscreen (tela).");
    printf("\n\tlist - imprimi lista de usuários ativos.");
    printf("\n\thelp - consulta ajuda.");
    printf("\n\tquit - encerra conexão com logins e deleta sala.");
    
    printf("\n\tremove - remove um usuário.\n\n");    
}

int menuComando(char *buffer){
    
    Comando *bloco = split(buffer);
    if(!bloco->lenghtComando){
        
        return 1;
    }
    if(!strncmp(bloco->comando, "add", 4)){
               
        inserirUsuario(listaLogin);
    }else if(!strncmp(bloco->comando, "remove", 7)){
        if(bloco->lenghtParametro){        
            removerUsuario(listaLogin, bloco->parametro);
        }else{
            
            printf("Erro na exlusão.\nSintaxe $ remove NICK.\n\n");
        }    
    }else if(!strncmp(bloco->comando, "list", 5)){
        
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

void menuMenssagem(char *buffer){
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMenssagem(buffer);
    
    //printf("valor de retorno %d\n", strncmp(bloco->parametro, "-help", 6));
    if(!strncmp(bloco->parametro, "-help", 6)){
        
        ajudaMenssagem();
        return;
    }else if(!strncmp(bloco->parametro, "-clear", 7)){
        
        system("clear");
        return;
    }else if(!strncmp(bloco->parametro, "-list", 6)){
        
        imprimirLista(listaLogin);
        return;
    }
    
    // Enviando a menssagem.
    // se for uma mensagem para broadcast.
    if(!strncmp(bloco->comando, "all", 4)){ 
        
        Link aux = listaLogin->primeiro;
        while(aux != NULL){
            
            pthread_t t;
            pthread_create(&t, NULL, (void *) enviarMenssagem, NULL /*aux*/);            
            pthread_join(t, NULL);
            aux = aux->prox;
        }
    }else{ // Enviando mensagem unicast.
        
        // passando o nick específico para pesquisa.
        Link aux = pesquisarNick(listaLogin, bloco->comando);
        if(aux != NULL){
            
            pthread_t t;
            pthread_create(&t, NULL, (void *) enviarMenssagem, NULL /*aux*/);            
            pthread_join(t, NULL);
            return;
        }
        printf("Usuário inexistente.\n");
    }   
}

void menuOperacao( ){
    
    listaLogin = iniciarLista();
    char *buffer;// = calloc(sizeof(char),251);
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
            if(buffer[0] != '!' || buffer[1] != 'c') {
                                
                if(!menuComando(buffer)){
                    
                    exit(0);
                }
                
            }            
            
        }else{ // Modo de envio de menssagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!'|| buffer[1] != 'm') {
                
                menuMenssagem(buffer);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

void servidorMenssagem(){
    
    // enquanto o main estiver ativo.
    // pensando em possivelmente passar um parâmetro que indica termino
    // para esta função.
    int socketLocal, socketCliente, sizeSockaddr, *novoSocket;
    struct sockaddr_in servidor, cliente;
    
    // Criando um socket para tcp no sistema.
    // AF_INET tipo de conexão sobre IP para redes.
    // SOCK_STREAM protocolo com controle de erros.
    // 0 seleção do protocolo TCP
    socketLocal = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socketLocal == -1){
        
        printf("Erro ao abrir conexão.\n");
        return;
    }
        
    servidor.sin_family = AF_INET; // Atribuindo a familia de protocolos para Internet
    servidor.sin_addr.s_addr = INADDR_ANY; // Setando IP local.
    servidor.sin_port = htons(7772); // Setando e porta em que rodara o processo.
    
    
    // Criano link entre a estrutura servidor ao ID do socketLocal.
    if(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro no bind.\n");
        return;
    }
    
    // Limitando o número de conexões que o socket local vai ouvir para 15.
    listen(socketLocal, 15);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    
    // Esperando por conexões.
    // Faça enquanto existir conexões. (socklen_t*)&c)
    while(socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr) < 0){
        
        // Criando uma thread para um cliente.
        // A nova thread ficara responsável por enviar as mensagens do cliente
        // mantendo a conexão até o término da execução.
        pthread_t threadCliente;
        novoSocket = malloc(1);
        *novoSocket = socketCliente;
        
        if(pthread_create(&threadCliente, NULL, enviarMenssagem, (void *) novoSocket)){
            
            printf("Erro ao criar a thread.\n");
            return;
        }        
        printf("thread cliente criada.\n");
    }
    if(socketCliente < 0){
        
        printf("Falha na função accpet.\n");
    }
}

void *enviarMenssagem(){
    
    
    printf("Mensagem enviada.\n");
}