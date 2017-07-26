/*
 * 
 Implementação da "tabela" de login utilizando lista duplamente encadeada.
 * 
 */

#ifndef usuario_H
#define usuario_H

#include "strUtils.h"

// Ponterio para célula de login.
typedef struct login *Link;

// Armazena o nome (apelido de usuario e seu endereço IP).
typedef struct login {
    
    char *nick;
    char *ip;    
    Link prox;
} Login;

// Descritor auxilia a Trabalhar com operações na lista.
typedef struct descritor {

    Link primeiro;
    Link ultimo;
    int tamanho;
} Descritor;

// Lista global de usuários
// Necessária para trabalho com threads.
Descritor *listaLogin;

// Verifica se a lista de logins esta vazia.
int listaVazia();

// Inicia o descritor da lista.
Descritor *iniciarLista();

// Insere uma nova celula na tabela de logins com seu respectivo endereço IP.
int inserirUsuario(Descritor *listaLogin);

// Verifica se nick existe na tabela de usuários e retorna o endereço de sua posição na lista.
// Caso o usuário requerido não exista na lista é retornada nulo.
Link pesquisarNick(Descritor *listaLogin, char *nick);

// Verifica se o IP a ser inserido já esta sendo utilizado por outro host.
// Se sim é retornado o endereço de do login na lista.
// se não é retornado o valor NULL.
Link pesquisarIp(Descritor *listaLogin, char *ip);

// Realiza comparação de Nick e Ip ao mesmo tempo.
// Retorna 1 se encontrar nicks iguais.
// Retorna 2 se encontrar ips iguais.
// Zero se ip e nick não forem iguais.
int pesquisarNickIp(Descritor *listaLogin, char *nick, char *ip);

// Imprimi a lista de usuários ativos no servidor.
void imprimirLista(Descritor *listaLogin);

// Remove usuário da lista do server.
void removeUsuario(Descritor *listaLogin, char *nick);

#endif