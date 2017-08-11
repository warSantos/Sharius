#include "./servidor.h"

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
            
            if(removerUsuario(listaLogin, bloco->parametro)){
                
                printf("Usuário inexistente.\n");
                return 1;
            }
            printf("Usuário: %s removido.\n", bloco->parametro);
        }else{
            
            printf("Erro na exlusão.\nSintaxe $ remove NICK.\n\n");
        }    
    }else if(!strncmp(bloco->comando, "list", 5)){
        
        imprimirLista(listaLogin);
    }else if(!strncmp(bloco->comando, "quit", 5)){
        
        printf("Fechando conexões...\n");
        printf("Have a nice day....\n");
        fechaConexoes();
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

void menuMensagem(char *buffer, int idSocket) {
    
    // Comando vai ser utilizado com identificador de usuário por padão é (all).
    Comando *bloco = extraiMensagem(buffer);    
    if(!bloco->parametro){
        
        return;
    }
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
    enviarStr(idSocket, buffer);
}

void menuOperacao(int idSocket){
    
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
                
                menuMensagem(buffer, idSocket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

void addUserRemoto(Descritor *listaLogin, char *nick, int *sock){
    
    size_t len = strlen(nick)+1;
    Link aux = malloc(sizeof(Login));
    aux->nick = malloc(sizeof(char) * len);        
    aux->socket = sock;    
    strncpy(aux->nick, nick, len);            
    
    // Caso seja a primeira inserção de um usuário na lista.
    if(listaVazia(listaLogin)){
        
        listaLogin->primeiro = aux;
        listaLogin->ultimo = aux;
        aux->prox = NULL;           
    }else{
    
        listaLogin->ultimo->prox = aux;
        listaLogin->ultimo = aux;        
    }    
    printf("PRIMEIRA INSERÇÃO %s .\n", aux->nick);
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
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(40001); // Setando e porta em que rodara o processo.       
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    // Criando link entre a estrutura servidor ao ID do socketLocal.
    if(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro no bind.\n");
        close(socketLocal);
        //usleep(1000000);
        //return;
        exit(1);
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
                        
        pthread_t threadCliente;
        novoSocket = malloc(1);
        *novoSocket = socketCliente;     
        
        pthread_mutex_lock(&lista);
        // Possível local para implantação de mutex.        
        printf("--> %s tl: %d\n", nick, listaLogin->tamanho);
        addUserRemoto(listaLogin, nick, novoSocket);
        printf("--> %s tl: %d\n", nick, listaLogin->tamanho);
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

void *escutaCliente(void *socketCliente){
    
    //Id de ientificação do cliente que utiliza a função
    // Podem ser várias threads desta liberadas
    // Logo o idSocket de cada chamada da função escutaCliente 
    // é diferente é um socket diferente.
    
    int idSocket = *(int*)socketCliente, read_size;    
    //int read_size;
    
    // enviando confirmação de pronto para receber nome...
    char *buffer, *donoThread, ac = 'S';                
    write(idSocket, &ac, 1);
    
    // recebendo o nome do dono da thread.
    if(recebeStr(idSocket, &donoThread) <= 0){
    
        printf("Erro ao receber nome na thread de escuta individual.\n");
        return 0;
    }
    
    // recebe mensagens do cliente.
    while((read_size = recebeStr(idSocket, &buffer)) > 0){
        
        // extraindo cliente para envio.
        Comando *bloco = extraiMensagem(buffer);        
        // Repassa para broadcast.
        if (!strncmp(bloco->comando, "all", 4)) {

            Link aux = listaLogin->primeiro;
            while (aux != NULL) {
                
                // não enviar para o mesmo que recebeu.
                if(strcmp(aux->nick, donoThread)){
                
                    enviarBloco(buffer, donoThread, *aux->socket);
                }                
                aux = aux->prox;
            }            
        } else { // Enviando mensagem unicast.

            // passando o nick específico para pesquisa.
            Link aux = pesquisarNick(listaLogin, bloco->comando);
            if (aux != NULL) {
                                
                enviarBloco(buffer, donoThread, *aux->socket);
                
            }else{
                
                // Retorna mesagem de erro.
                //char *userNotExist = "Usuário inexistente.\n";
                //write(idSocket , userNotExist , strlen(userNotExist)+1);
            }
        }
    }            
    if(read_size == 0){
                
        pthread_mutex_lock(&lista);
        // Possível local para implantação de mutex.        
        removerUsuario(listaLogin, donoThread);                    
        pthread_mutex_unlock(&lista);
        fflush(stdout);
        
    }else if(read_size == -1){
        
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socketCliente);
    printf("fechando conexão com cliente...\n");
    return 0;        
}

int abreConexaoLocal(char **userNick, char *senha){
        
    struct sockaddr_in servidor;    
     
    //Create socket
    int retSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (retSocket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return retSocket;
    } 
    
    char *ip = "127.0.0.1";            
    
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr(ip);
    
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(40001);
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor...
    
    if (connect(retSocket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return retSocket;
    }                    
    
    // Criar usuário.        
    char ok;               
    char *nick;       
                            
    size_t len = strlen(senha) + 1;
    char lenght = retChar(len);

    // Enviando o tamanho da senha.
    write(retSocket, &lenght, 1);
    //printf("LENGHT: %d\n", (int)lenght);
    // enviando a senha.
    write(retSocket, senha, len);
    //printf("SENHA: %s\n", senha);
    // Recebendo confiramção.
    recv(retSocket, &ok, sizeof (char), 0);                                            
    if(ok != 'S'){
        
        printf("Erro no password.\n");
        return -1;
    }
    
    while(1){
                
        nick = criaNick();
        size_t len = strlen(nick) + 1;
        lenght = retChar(len);
        
        // Enviando o tamanho do nick.
        write(retSocket, &lenght, 1);       
        //printf("LENGHT: %d\n", (int)lenght);
        
        // enviando login para aprovação...
        write(retSocket, nick, len);
        //printf("NICK: %s\n", nick);
        
        // recebendo confirmação...
        recv(retSocket, &ok, 1, 0);
        //printf("OK: %c\n", ok);
        if(ok == 'S'){
            
            break;
        }
        printf("Este login já esta em uso.\n\n");
    }        
    
    char ac;
    recv(retSocket, &ac, 1, 0);
    
    // enviando login para a thread de escutaCliente.
    enviarStr(retSocket, nick);
    
    len = (strlen(nick)+1);
    *userNick = malloc(sizeof(char)* len);
    strncpy(*userNick, nick, len);
    
    free(nick);
    printf("Login cadastrado %s .\n", *userNick);                
    printf("listaLogin tamanho: %d\n", listaLogin->tamanho);
    return retSocket;
}

void fechaConexoes(){
        
    if(!listaVazia(listaLogin)){
        
        Link aux = listaLogin->primeiro;
        while(aux != NULL){

            shutdown(*aux->socket, 2);
            removerUsuario(listaLogin, aux->nick);
            aux = aux->prox;
        }        
    }
    free(listaLogin);
}