#include "../headers/usuario.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* FUNÇÕES PARA TRABALHO DE GERENCIAMENTO DE USUÁRIO! */

int listaVazia(){
    
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

char *retornaNick(char *nick){
    
    Link aux = pesquisarNick(nick);
    if(aux != NULL){ // Existe usuário e seu nick
                     // esta sendo retornado.
        return aux->nick;
    }
    // Não existe usuário então retornamos NULL.
    return NULL;
}

int inserirUsuario(){
        
    char *nick;
    
    // Verificação se o Login é válido.
    while(1){
        
        // Se o nick for válido.
        if((nick = criaNick()) != NULL){
            
            // Se o nick não for existente na lista de login.
            if(pesquisarNick(nick) != NULL){                                                            
                
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
    if(listaVazia()){
        
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

Link pesquisarNick(char *nick){
    
    Link aux = listaLogin->primeiro;
    while(aux != NULL){
        
        if(!strcmp(aux->nick, nick)){
            
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void imprimirLista(){
    
    if(listaVazia()){
        
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

int removerUsuario(char *nick){
    
    if(listaVazia()){
        
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

void enviarMensagem(char *buffer, int idSocket){                
    
    // enviando a mensagem para o cliente.    
    write(idSocket , buffer , strlen(buffer) + 1);
}

void enviarStr(int idSocket, char *str){
    
    int len = strlen(str) + 1;
    char *lenght = retChar(len);
    
    // enviando o tamanho do nick.
    write(idSocket, lenght, 4);
    
    // enviando o login.
    write(idSocket, str, retInt(lenght));    
}

int recebeStr(int idSocket, char **donoThread){
        
    char *lenght = malloc(4);    
    
    // recebendo o tamanho do nick
    int read = recv(idSocket, lenght, 4, 0);  
    int len = retInt(lenght);        
    if(read <= 0){
                
        return read;
    }    
    
    // recebendo o nick.
    *donoThread = malloc(len);
    read = recv(idSocket, *donoThread, len, 0);
    
    if(read <= 0){
                
        return read;
    }    
    return read;
}

void enviarBloco(char *buffer, char *login, int sock){

    // enviando o tamanho do size login do emissor.
    int len = strlen(login) + 1;
    char *lenght = retChar(len);        
    
    write(sock , lenght , 4);
    
    // enviando o nick    
    write(sock, login, len);
    
    // enviando a mensagem.
    write(sock, buffer, 2048);   
}

int recebeBloco(char** buffer, char** nickEmissor, int idSocket){
                
    int read_size;
    *buffer = malloc(sizeof(char)*2048);
    // Recebendo o size do login.
        
    char *lenght = malloc(4);
    read_size = recv(idSocket, lenght, 4, 0);
    if (read_size <= 0) {

        return read_size;
    }

    int len = retInt(lenght);
    *nickEmissor = malloc(len);

    // Recebendo o login.
    read_size = recv(idSocket, *nickEmissor, len, 0);
    if (read_size <= 0) {

        return read_size;
    }

    // Recebendo a mensagem.
    read_size = recv(idSocket, *buffer, 2048, 0);
        
    return read_size;
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
        
        puts("Desconectando...");
        fflush(stdout);        
    }else if(read_size == -1){
        
        perror("recv failed");
    }
         
    //fechando a conexão
    printf(ANSI_COLOR_RED "Servidor inoperante..." ANSI_COLOR_RESET " \n");
    close(idSocket);        
}