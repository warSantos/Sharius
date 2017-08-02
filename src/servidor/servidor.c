#include "./servidor.h"
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

void menuMensagem(char *buffer, int socket) {
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMensagem(buffer);
    
    //printf("valor de retorno %d\n", strncmp(bloco->parametro, "-help", 6));
    if (!strncmp(bloco->parametro, "-help", 6)) {

        ajudaMensagem();
        return;
    } else if (!strncmp(bloco->parametro, "-clear", 7)) {

        system("clear");
        return;
    } else if (!strncmp(bloco->parametro, "-list", 6)) {

        imprimirLista(listaLogin);
        return;
    }   
    
    
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
                
                menuMensagem(buffer, socket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

void addUserRemoto(Descritor *listaLogin, char *nick, char *ip, int *sock){
    
    Link aux = malloc(sizeof(Login));
    aux->nick = malloc(sizeof(char) * 16);    
    aux->ip = malloc(sizeof(char) * 16);        
    aux->socket = sock;
    printf("user: %s aux->sock: %d\n", nick, *aux->socket);
    strncpy(aux->nick, nick, 16);
    strncpy(aux->ip, ip, 16);        
    free(nick);
    free(ip);
    
    // Caso seja a primeira inserção de um usuário na lista.
    if(listaVazia(listaLogin)){
        
        listaLogin->primeiro = aux;
        listaLogin->ultimo = aux;
        aux->prox = NULL;      
    }else{
    
        listaLogin->ultimo->prox = aux;
        listaLogin->ultimo = aux;        
    }    
    listaLogin->tamanho++;    
}

// Versão para uso real
void escutaSolicitacao(void *password){
    
    char *senha = (char *)password;            
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
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");//inet_addr("127.0.0.1"); // Setando IP local.
    servidor.sin_port = htons(7772); // Setando e porta em que rodara o processo.       
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    // Criando link entre a estrutura servidor ao ID do socketLocal.
    while(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro no bind.\n");
        close(socketLocal);
        //return;
    }    
    // Limitando o número de conexões que o socket local vai ouvir para 15.
    listen(socketLocal, 15);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    
    // Esperando por conexões.
    // Faça enquanto conexões forem solicitadas.
    while(1){
        
        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        if(socketCliente < 0){
            
            break;
        }
        // Criando uma thread para um cliente.
        // A nova thread ficara responsável por enviar as mensagens do cliente
        // mantendo a conexão até o término da execução.                
        
        // Capturando senha do cliente.
        int tentativas = 0, len; 
        char *senhaTemp, lenght;
        while(tentativas < 3){
            
            // Recebendo o tamanho da senha.
            recv(socketCliente, &lenght, 1, 0);
            len = retInt(lenght);
            senhaTemp = malloc(sizeof(char)*len);
            
            // Recebendo a senha.
            recv(socketCliente, senhaTemp, len, 0);                        
            printf("SENHA TEMP: %s.\n\n", senhaTemp);
            tentativas++;
            char ok;
            if(!strncmp(senhaTemp, senha, strlen(senhaTemp))){
                
                ok = 'S';
                write(socketCliente, &ok, 1);
                break;
            }
            ok = 'N';
            write(socketCliente, &ok, 1);
        }
        free(senhaTemp);
        
        char *nick;
        Link aux;
        while(1){
            
            // Recebendo o tamanho do nick.
            recv(socketCliente, &lenght, 1, 0);
            len = retInt(lenght);            
            nick = malloc(sizeof(char)*len);
            
            // Recebendo o nick
            recv(socketCliente, nick, len, 0);            
            aux = pesquisarNick(listaLogin, nick);
            char ok;
            if (aux == NULL) { // se login nao existir
                
                ok = 'S';
                write(socketCliente, &ok, 1);
                break;
            }                    
            ok = 'N';
            write(socketCliente, &ok, 1);
        }
        char *ip;
        while(1){
            
            recv(socketCliente, &lenght, 1, 0);
            len = retInt(lenght);
            ip = malloc(sizeof(char)*len);
            
            recv(socketCliente, ip, len, 0);            
            aux = pesquisarIp(listaLogin, ip);
            char ok;
            if (aux == NULL) { // se login nao existir
                
                ok = 'S';
                write(socketCliente, &ok, 1);
                break;
            }                    
            ok = 'N';
            write(socketCliente, &ok, 1);
        }
                
        pthread_t threadCliente;
        novoSocket = malloc(1);
        *novoSocket = socketCliente;     
        
        pthread_mutex_lock(&lista);
        // Possível local para implantação de mutex.
        addUserRemoto(listaLogin, nick, ip, novoSocket);
        pthread_mutex_unlock(&lista);
        
        if(pthread_create(&threadCliente, NULL, escutaCliente, (void *) novoSocket)){
            
            printf("Erro ao criar a thread.\n");
            return;
        }                
    }
    if(socketCliente < 0){
        
        printf("Falha na função accpet.\n");
    }
}

void *escutaCliente(void *idSocket){
    
    //Id de ientificação do cliente que utiliza a função
    // Podem ser várias threads desta liberadas
    // Logo o idSocket de cada chamada da função escutaCliente 
    // é diferente é um socket diferente.
    
    int socket = *(int*)idSocket;    
    int read_size;
    char *buffer = malloc(sizeof(char) * 251);
    
    // recebe mensagens do cliente.
    while( (read_size = recv(socket, buffer, 251 , 0)) > 0 ){
        
        printf("MENSAGEM NO SERVIDOR: %s \n", buffer);        
        
        // extraindo cliente para envio.
        Comando *bloco = extraiMensagem(buffer);
        
        // Repassa para broadcast.
        if (!strncmp(bloco->comando, "all", 4)) {

            Link aux = listaLogin->primeiro;
            while (aux != NULL) {

                pthread_t t;
                pthread_create(&t, NULL, (void *) enviarMensagem, NULL);
                pthread_join(t, NULL);
                aux = aux->prox;
            }            
        } else { // Enviando mensagem unicast.

            // passando o nick específico para pesquisa.
            Link aux = pesquisarNick(listaLogin, bloco->comando);
            if (aux != NULL) {
                
                // enviando a mensagem.
                //enviarMensagem(buffer, socket);
                
            }else{
                
                // Retorna mesagem de erro.
                char *userNotExist = "Usuário inexistente.\n";
                write(socket , userNotExist , strlen(userNotExist));
            }
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
     
    return 0;        
}

void enviarMensagem(char *buffer, int socket){
    
    write(socket , buffer , strlen(buffer));
}

int abreConexao(){
/*    
    int sock;
    struct sockaddr_in servidor;    
     
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
 */
    return 0;
}