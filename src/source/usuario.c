#include "../headers/usuario.h"

/* FUNÇÕES PARA TRABALHO DE GERENCIAMENTO DE USUÁRIO! */

int listaVazia(Descritor *listaLogin){
    
    // listaLogin->tamanho > 0
    if(listaLogin->tamanho){
        
        return 0;
    }else{
        
        return 1;
    }
}

Descritor *iniciarLista(){
        
    Descritor *aux = malloc(sizeof(Descritor));
    aux->primeiro = NULL;
    aux->ultimo = NULL;
    aux->tamanho = 0;
    return aux;
}

char *criaNick(){
    
    char *nick = (char *) malloc(sizeof(char) * 16);        
    
    // Verificação se o Login é válido.
    while(1) {
        
        printf("Atenção o nick não devem possuir caracteres\nespeciais como: !, # , $, &, *, (, ), ^, e espaço...\n");    
        printf("Sendo permitido somente o letras, numeros e _.\n");
        printf("Limite máximo de caracteres 15.\n");        
        printf("Digite um nick: ");
        scanf("%15[^\n]s", nick);
        __fpurge(stdin);
        
        // verificando se o nick é válido.
        char *invalidos = " !$%¨&*().,;^~][{}+º°?ª<>/|\\";
        if(!charInvalido(nick, invalidos)){
            
            return nick;
        }else{
        
            printf("Nick inválido!\n");
        }
        
        char op;
        while(1){
            
            printf("Tentar novo nick (S/n): ");
            scanf("%c", &op);
            __fpurge(stdin);
            if(op == 'S' || op == 's'){
                
                break;
            }else if(op == 'N' || op == 'n'){
                
                return NULL;
            }
        }                
    }
}

char *criaIp(){
    
    char *ip = (char *) malloc(sizeof(char) * 16);    
    
    while (1) {

        printf("Endereços IP devem estar da seguinte forma: 192.168.0.100 .\n");
        printf("Digite o endereço: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if (!verificaIp(ip)) {

            return ip;            
        }

        char op;
        while (1) {

            printf("Deseja reinserir o ip (S/n): ");
            scanf("%c", &op);
            __fpurge(stdin);
            if (op == 'S' || op == 's') {

                break;
            } else if (op == 'N' || op == 'n') {

                return NULL;
            }
        }                
    }
}

char *retornaNick(Descritor *listaLogin, char *nick){
    
    Link aux = pesquisarNick(listaLogin ,nick);
    if(aux != NULL){ // Existe usuário e seu nick
                     // esta sendo retornado.
        return aux->nick;
    }
    // Não existe usuário então retornamos NULL.
    return NULL;
}

int inserirUsuario(Descritor *listaLogin){
        
    char *nick;// = (char *) malloc(sizeof(char) * 16);        
    
    // Verificação se o Login é válido.
    while(1){
        
        // Se o nick for válido.
        if((nick = criaNick()) != NULL){
            
            // Se o nick não for existente na lista de login.
            if(pesquisarNick(listaLogin, nick) != NULL){                                                            
                
                printf("Este login esta indisponível no momento.\n");
            }
        }else{
        
            printf("Nick inválido!\n");
        }        
        char op;
        while(1){
            
            printf("Deseja reinserir um novo Login (S/n): ");
            scanf("%c", &op);
            __fpurge(stdin);
            if(op == 'S' || op == 's'){
                
                break;
            }else if(op == 'N' || op == 'n'){
                
                return 1;
            }
        }                
    }        
    // Fim de verificação.
    
    Link aux = malloc(sizeof(Login));
    aux->nick = malloc(sizeof(char) * 16);        
    strncpy(aux->nick, nick, 16);    
    free(nick);    
    
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
    printf("Usuário adicionado com sucesso!.\n.\n");    
    return 0;
}

Link pesquisarNick(Descritor *listaLogin, char *nick){
    
    Link aux = listaLogin->primeiro;
    while(aux != NULL){
        
        if(!strcmp(aux->nick, nick)){
            
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void imprimirLista(Descritor *listaLogin){
    
    if(listaVazia(listaLogin)){
        
        printf("Não existe usuários no chat.\n");
        return;
    }
    printf("Usuários online: %d.\n\n", listaLogin->tamanho);
    Link aux = listaLogin->primeiro;               
    printf("Usuários Online.\n");
    
    while(aux != NULL){
        
        printf("> %s \n", aux->nick);
        aux = aux->prox;
    }
}

int removerUsuario(Descritor *listaLogin, char *nick){
    
    if(listaVazia(listaLogin)){
        
        printf("\n\nNão há usuários Online.\n\n");
        return 1;
    }
    Link aux = listaLogin->primeiro;    
    // Caso o usuario a ser removido seja o último da lista.
    if(!strcmp(nick, aux->nick)){
                
        listaLogin->primeiro = aux->prox;        
        listaLogin->tamanho--;
        free(aux);
        return 0;
    }
    Link aux2 = listaLogin->primeiro;
    // Caso não seja o primeiro elemento.
    while(aux != NULL){
        
        aux = aux->prox;
        if(aux != NULL && !strcmp(nick, aux->nick)){
            
            if(aux == listaLogin->ultimo){
                
                listaLogin->ultimo = aux2;
            }
            aux2->prox = aux->prox;                        
            listaLogin->tamanho--;
            free(aux);
            return 0;
        }
        aux2 = aux2->prox;  
    }    
    return 1;
}

/* FUNÇÕES PARA TRABALHO COM SOCKETS! */

int abreConexao(char **userNick){
        
    struct sockaddr_in servidor;    
     
    //Create socket
    int retSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (retSocket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return retSocket;
    } 
    
    //char *ip = "127.0.0.1";
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
    servidor.sin_port = htons(40001);
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor...
    
    if (connect(retSocket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return retSocket;
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
        write(retSocket, &lenght, 1);       
        
        // Devolvendo senha.
        write(retSocket, resposta, len);        
        
        // Recebendo confiramção.
        recv(retSocket, &ok, sizeof (char), 0);
        
                
        if (ok == 'S') { // se receber acesso autorizado.
                         // sai do loop e vai para criação de usuário.
            break;
        }        
        printf("Senha errada.\n");
        tentativas++;
    }
    if(tentativas == 3){
        
        printf("Conexão perdida.\n\n");
        close(retSocket);
        return -1;
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
        write(retSocket, &lenght, 1);       
        
        // enviando login para aprovação...
        write(retSocket, nick, len);
        
        // recebendo confirmação...
        recv(retSocket, &ok, 1, 0);
        if(ok == 'S'){
            
            break;
        }
        printf("Este login já esta em uso.\n\n");
    }        
    
    // enviando login para a thread de escutaCliente.
    enviarStr(retSocket, nick);
    
    *userNick = malloc(sizeof(char)* (strlen(nick)+1));
    strncpy(*userNick, nick, (strlen(nick)+1));
    
    free(nick);
    printf("Login cadastrado...\n");                
    return retSocket;
}

void enviarMensagem(char *buffer, int idSocket){                
    
    // enviando a mensagem para o cliente.    
    write(idSocket , buffer , strlen(buffer) + 1);
}

void enviarStr(int idSocket, char *str){
    
    char lenght = retChar(strlen(str) + 1);    
    // enviando o tamanho do nick.
    write(idSocket, &lenght, 1);
    write(idSocket, str, retInt(lenght));
    // enviando o login.
}

int recebeStr(int idSocket, char **donoThread){
        
    char lenght;    
    // recebendo o tamanho do nick
    int read = recv(idSocket, &lenght, 1 , 0);        
    if(read <= 0){
                
        return read;
    }
    // recebendo o nick.
    *donoThread = malloc(sizeof(char)*retInt(lenght));
    read = recv(idSocket, *donoThread, retInt(lenght), 0);
    if(read <= 0){
                
        return read;
    }
    return read;
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

int recebeBloco(char** buffer, char** nickEmissor, int idSocket){
                
    int read_size;
    *buffer = malloc(sizeof(char)*251);
    //buffer = malloc(sizeof(char) * 251);
    
    // Recebendo o size do login.
        
        char lenght;
        read_size = recv(idSocket, &lenght, 1 , 0);        
        if(read_size <= 0){
            
            return read_size;
        }
        
        int len = retInt(lenght);
        *nickEmissor = malloc(sizeof(char)*len);
        //nickEmissor = malloc(sizeof(char)*len);
        
        // Recebendo o login.
        read_size = recv(idSocket, *nickEmissor, len , 0);
        if(read_size <= 0){
            
            return read_size;
        }
        // Recebendo a mensagem.
        read_size = recv(idSocket, *buffer, 251 , 0);
        
    return read_size;
}