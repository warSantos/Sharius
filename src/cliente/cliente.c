#include "cliente.h"
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include<unistd.h>    //write

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

void menuMensagem(char *buffer, char *userNick, int socket){
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMensagem(buffer);
    
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
    
    enviarBloco(buffer, userNick, socket);    
}

void enviarMensagem(char *buffer, int socket){                
    
    // enviando a mensagem para o cliente.    
    write(socket , buffer , strlen(buffer) + 1);
}

void enviarBloco(char *buffer, char *login, int sock){

    // enviando o tamanho do size login do emissor.
    char lenght = retChar(strlen(login) + 1);    
    int len = retInt(lenght);    
    write(sock , &lenght , 1);
    
    // enviando o nick    
    write(sock, login, len);
    
    // enviando a mensagem.
    write(sock, buffer, 251);   
}

void menuOperacao(char *userNick, int socket){
    
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
                
                menuMensagem(buffer, userNick, socket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

char *abreConexao(int *retSocket){
        
    struct sockaddr_in servidor;    
     
    //Create socket
    *retSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (*retSocket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return NULL;
    } 
    
    char *ip = "127.0.0.1", *userNick;
    /*
    char *ip = malloc(sizeof(char)*16), *userNick;
    
    while(1){
        
        printf("Digite o ip do servidor: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if(!verificaIp(ip)){
            
            break;
        }
    }        
    */
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr(ip);
    
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(7772);
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor...
    
    if (connect(*retSocket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return NULL;
    }                    
    
    // Cria usuário.
    // Recebe mensagem de autenticação.
    int tentativas = 0;
    char ok;
    while(tentativas < 3){
        
        char *resposta = "123";
        /*
        printf("Senha de acesso: ");

        // Recebendo senha do teclado.
        char *resposta = malloc(sizeof(char)*16);
        
        scanf("%15[^\n]s", resposta);
        __fpurge(stdin);        
        */
        size_t len = strlen(resposta) + 1;
        char lenght = retChar(len);
        
        // Enviando o tamanho da senha.
        write(*retSocket, &lenght, 1);       
        
        // Devolvendo senha.
        write(*retSocket, resposta, len);        
        
        // Recebendo confiramção.
        recv(*retSocket, &ok, sizeof (char), 0);

        if (ok == 'S') { // se receber acesso autorizado.
                         // sai do loop e vai para criação de usuário.
            break;
        }        
        printf("Senha errada.\n");
        tentativas++;
    }
    if(tentativas == 3){
        
        printf("Conexão perdida.\n\n");
        close(*retSocket);
        return NULL;
    }
    printf("\n\n");
    // criando usuário.
    
    char *nick;// = malloc(sizeof(char)*16);
   // free(ip);// = malloc(sizeof(char)*16);
    
    while(1){
                
        nick = criaNick();
        size_t len = strlen(nick) + 1;
        char lenght = retChar(len);
        
        // Enviando o tamanho do nick.
        write(*retSocket, &lenght, 1);       
        
        // enviando login para aprovação...
        write(*retSocket, nick, len);
        
        // recebendo confirmação...
        recv(*retSocket, &ok, 1, 0);
        if(ok == 'S'){
            
            break;
        }
        printf("Este login já esta em uso.\n\n");
    }        
    
    userNick = malloc(sizeof(char)* (strlen(nick)+1));
    strncpy(userNick, nick, (strlen(nick)+1));
    
    free(nick);
    printf("Login cadastrado...\n");                
    return userNick;
}

void recebeMensagem(void *idSocket){
    
    //Id de ientificação do cliente que utiliza a função
    // Podem ser várias threads desta liberadas
    // Logo o idSocket de cada chamada da função escutaCliente 
    // é diferente é um socket diferente.
    printf("Servidor de mensagens pronto...\n\n");
    
    int read_size;    
    int socket = *(int*) idSocket;
    char *buffer = malloc(sizeof(char)*251), lenght, *userNick;
    
    // recebe mensagens do cliente.
    while(1){
        
        // Recebendo o tamanho do login.
        read_size = recv(socket, &lenght, 1 , 0);
        if(read_size < 0){
            
            break;
        }
        
        // Reebendo o userNick.
        int len = retInt(lenght);
        userNick = malloc(sizeof(char)*len);        
        read_size = recv(socket, userNick, len , 0);                        

        // recebendo a mensagem.
        read_size = recv(socket, buffer, 251 , 0);
        
        // extraindo mensagem do buffer recebido.
        Comando *bloco = extraiMensagem(buffer);
        
        // Identifica quem enviou.
        if (!strncmp(bloco->comando, "all", 4)) {

           printf("%s-:> %s\n",userNick, bloco->parametro);             
        } else { // Enviando mensagem unicast.

            printf("%s-:> %s\n",userNick, bloco->parametro);
        }
    }            
    if(read_size == 0){
        
        puts("Client disconnected");
        fflush(stdout);        
    }else if(read_size == -1){
        
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(idSocket);
}