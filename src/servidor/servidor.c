#include "./servidor.h"
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
    printf("\tTODA mesagem iniciada por @ será comprendido como uma mensagem privada.\n");
    printf("\tEntão evite iniciar mesagens com @ com destino para broadcast (todos na sala).\n");
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

void menuMenssagem(char *buffer, int socket) {
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMenssagem(buffer);
    
    //printf("valor de retorno %d\n", strncmp(bloco->parametro, "-help", 6));
    if (!strncmp(bloco->parametro, "-help", 6)) {

        ajudaMenssagem();
        return;
    } else if (!strncmp(bloco->parametro, "-clear", 7)) {

        system("clear");
        return;
    } else if (!strncmp(bloco->parametro, "-list", 6)) {

        imprimirLista(listaLogin);
        return;
    }   
    
    /* Aqui entrara a função de envio de mensagem...
    //Envia menssagem...
    if (send(socket, message, strlen(message), 0) < 0) {

        puts("Send failed");
        return;
    }   
    // close(sock);        
     */
}

void menuOperacao(int socket){
    
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
        }else{ // Modo de envio de mesagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!'|| buffer[1] != 'm') {
                
                menuMenssagem(buffer, socket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

// Versão para uso real
void servidorMenssagem(){
    
    printf("Iniciando servidor de mensagem...\n\n");
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
        
        printf("Erro ao criar socket local.\n");
        return;
    }
        
    servidor.sin_family = AF_INET; // Atribuindo a familia de protocolos para Internet
    servidor.sin_addr.s_addr = INADDR_ANY; // Setando IP local.
    servidor.sin_port = htons(7772); // Setando e porta em que rodara o processo.       
    
    // Criando link entre a estrutura servidor ao ID do socketLocal.
    if(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro no bind.\n");
        return;
    }
    
    // Limitando o número de conexões que o socket local vai ouvir para 15.
    listen(socketLocal, 15);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    
    // Esperando por conexões.
    // Faça enquanto existir conexões.
    while((socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr)) < 0){
        
        // Criando uma thread para um cliente.
        // A nova thread ficara responsável por enviar as mensagens do cliente
        // mantendo a conexão até o término da execução.        
        pthread_t threadCliente;
        novoSocket = malloc(1);
        *novoSocket = socketCliente;
        
        if(pthread_create(&threadCliente, NULL, repassarMenssagem, (void *) novoSocket)){
            
            printf("Erro ao criar a thread.\n");
            return;
        }        
        printf("thread cliente criada.\n");
    }
    if(socketCliente < 0){
        
        printf("Falha na função accpet.\n");
    }
}

void *repassarMenssagem(void *idSocket){
    
    //Id de ientificação so softtware.
    int sock = *(int*)idSocket;
    int read_size;
    char *message , client_message[2000];
     
    //Menssagem de boas vindas
    message = "Acesso permitido...\n";
    write(sock , message , strlen(message));
     
    message = "Seja bem vindo ao chat... \n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0){
        
        puts("Client disconnected");
        fflush(stdout);        
    }else if(read_size == -1){
        
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(idSocket);
     
    return 0;
    
    /* Função deve ser movida para a função servidor de mensagem...
     *  Ou para a função de repassar mensagem...
     *  Atenção para os parametros na criação das threads.
    
    
    // Enviando a mesagem.
    // se for uma mensagem para broadcast.
    if(!strncmp(bloco->comando, "all", 4)){ 
        
        Link aux = listaLogin->primeiro;
        while(aux != NULL){
            
            pthread_t t;
            pthread_create(&t, NULL, (void *) repassarMenssagem, NULL);            
            pthread_join(t, NULL);
            aux = aux->prox;
        }
    }else{ // Enviando mensagem unicast.
        
        // passando o nick específico para pesquisa.
        Link aux = pesquisarNick(listaLogin, bloco->comando);
        if(aux != NULL){
            
            pthread_t t;
            pthread_create(&t, NULL, (void *) enviarMenssagem, NULL);            
            pthread_join(t, NULL);
            return;
        }
        printf("Usuário inexistente.\n");
    } 
    */
}

int abreConexao(){
    
    int sock;
    struct sockaddr_in servidor;
    char message[1000] , servidor_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return sock;
    }    
     
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(7772);
 
    //Busca conexão com o servidor...
    if (connect(sock , (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
        
        perror("Erro. conexão não estabelecida...");
        return sock;
    }
    printf("Conexão realizada...\n");    
    return sock;
}