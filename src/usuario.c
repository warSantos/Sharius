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

void enviarStr(int idSocket, char *str){
    
    int len = strlen(str) + 1;
    char *lenght = retChar(len);
    
    // enviando o tamanho da string.
    write(idSocket, lenght, 4);
    
    // enviando a string.
    write(idSocket, str, retInt(lenght));    
}

Mensagem *recebeStr(int idSocket){
        
    Mensagem *msg = malloc (sizeof(Mensagem));
    char *lenght = malloc(4);
    
    // recebendo o tamanho da string.
    msg->bytes_read = recv(idSocket, lenght, 4, 0);
    if(msg->bytes_read <= 0){

        msg->msg = NULL;
        msg->lenght = 0;       
        return msg;
    }    
    msg->lenght = retInt(lenght);
    // recebendo a string.
    msg->msg = malloc(msg->lenght);
    msg->bytes_read = recv(idSocket, msg->msg, msg->lenght, 0);
    free (lenght);
    return msg;
}