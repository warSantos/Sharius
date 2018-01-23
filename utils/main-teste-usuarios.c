/*
 *
 * Arquivo main uilizado para debug dos modulos. 
 * 
 */

#include <stdio_ext.h>

#include "../headers/usuario.h"

int main(void){
    
    printf("Iniciando Server...\n");    
    Descritor *listaLogin = iniciarLista();
    
    int x = 0;            
    while(x < 5){
        
        printf("Inserção de usuário.\n\n");
        int rep = inserirUsuario(listaLogin);  
        if(rep){
            
            printf("Usuário NÃO adicionado...\n");
        }
        ++x;
    }
    x = 0;
    imprimirLista(listaLogin);
    while(x < 0){
        
        printf("Pesquise um nick: ");    
        char *nick = malloc(sizeof(char) * 16);    
        scanf("%15[^\n]s", nick);
        __fpurge(stdin);
        Link aux = pesquisarNick(listaLogin, nick);
        if(aux == NULL){
            
            printf("Usuário inexistente!\n");
        }else{
            
            printf("Usuário: %s endereço: %s.\n", aux->nick, aux->ip);
        }
        ++x;
        free(nick);
    }
    printf("Server iniciado com sucesso...\n");
    x = 0;
    while(x < 8){
        
        printf("Remova um usuário: ");    
        char *nick = malloc(sizeof(char) * 16);    
        scanf("%15[^\n]s", nick);
        __fpurge(stdin);
        removeUsuario(listaLogin, nick);
        imprimirLista(listaLogin);
        free(nick);
        ++x;
    }
    return 0;
}