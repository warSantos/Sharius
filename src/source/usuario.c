#include "../headers/usuario.h"
#include <stdio_ext.h>

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
        char *invalidos = "!$%¨&*().,;^~][{}+º°?ª<>/|\\";
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

char *retornaIp(Descritor *listaLogin, char *ip){
    
    Link aux = pesquisarIp(listaLogin, ip);
    if(aux != NULL){ // Já existe esse ip
                     // então ele esta sendo retornado.
        return aux->ip;
    }
    // Não existe colisão de ip então esta NULL esta sendo retorando.
    return NULL;
}

int inserirUsuario(Descritor *listaLogin){
        
    char *nick;// = (char *) malloc(sizeof(char) * 16);    
    char *ip;// = (char *) malloc(sizeof(char) * 16);    
    
    // Verificação se o Login é válido.
    while(1) {
        
        // Se o nick for válido.
        if((nick = criaNick()) != NULL){
            
            // Se o nick não for existente na lista de login.
            if(pesquisarNick(listaLogin, nick) == NULL){
                
                
                if((ip = criaIp()) != NULL){
                    
                    // Se o ip não estiver em uso.
                    if(pesquisarIp(listaLogin, ip) == NULL){
                        
                        break;
                    }
                    printf("Este ip pertence a outro host.\nPor favor verifique seu IP.\n");
                }
            }else{
                
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
    aux->ip = malloc(sizeof(char) * 16);        
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

Link pesquisarIp(Descritor *listaLogin, char *ip){
    
    Link aux = listaLogin->primeiro;
    while(aux != NULL){
        
        if(!strncmp(aux->ip, ip, 16)){
        
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

void removerUsuario(Descritor *listaLogin, char *nick){
    
    if(listaVazia(listaLogin)){
        
        printf("\n\nNão há usuários Online.\n\n");
        return;
    }
    Link aux = listaLogin->primeiro;    
    // Caso o usuario a ser removido seja o último da lista.
    if(!strcmp(nick, aux->nick)){
                
        listaLogin->primeiro = aux->prox;
        printf("\n\nUsuário %s removido com sucesso.\n\n", aux->nick);
        listaLogin->tamanho--;
        free(aux);
        return;
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
            printf("\n\nUsuário %s removido com sucesso.\n\n", aux->nick);
            listaLogin->tamanho--;
            free(aux);
            return;
        }
        aux2 = aux2->prox;  
    }    
    printf("Usuários online: %d.\n\n", listaLogin->tamanho);
    printf("\n\nEste usuário não esta na lista.\n\n");
}