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

void menuMensagem(char *buffer, int socket){
    
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
    
    /* 
     * Falta criar conexão e mandar mensagem... 
     */
}

void menuOperacao(int socket){
    
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
                
                menuMensagem(buffer, socket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

int abreConexao(){
    
    int sock;
    struct sockaddr_in servidor;    
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return sock;
    }    
     
    char *ip = malloc(sizeof(char)*16);
    while(1){
        
        printf("Digite o ip do servidor: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if(!verificaIp(ip)){
            
            break;
        }
    }
    
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr(ip);
    
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(7772);
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor...
    int newConnect;
    if ((newConnect = connect(sock, (struct sockaddr *)&servidor , sizeof(servidor))) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return newConnect;
    }            
    
    // Cria usuário.
    // Recebe mensagem de autenticação.
    int tentativas = 0;
    while(tentativas < 3){
        
        char *mensagemServer = "Senha de acesso: \n";
        char *resposta = malloc(sizeof (char) * 16);
        recv(sock, mensagemServer, strlen(mensagemServer), 0);
        printf("%s", mensagemServer);

        // Recebendo senha.
        scanf("%15[\n]s", resposta);
        __fpurge(stdin);
        
        // Devolvendo senha.
        write(sock, resposta, sizeof (resposta));
        char ok;
        
        // Recebendo confiramção.
        recv(sock, &ok, sizeof (char));

        if (ok == 'S') { // se receber acesso autorizado.
                         // sai do loop e vai para criação de usuário.
            break;
        }
        printf("Senha errada.\n")
        tentativas++;
    }
    if(tentativas == 3){
        
        printf("Conexão perdida.\n\n");
        close(sock);
        return sock = -1;
    }
    
    // Falta criar a função de criação de usuário.
    
    printf("Conexão realizada...\n");            
    
    return sock;
}

void recebeMensagem(){
    
    //printf("falta acabar.\n\n");
}