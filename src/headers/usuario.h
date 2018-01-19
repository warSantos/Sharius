/*
 * 
 Implementação da "tabela" de login utilizando lista duplamente encadeada.
 * 
 */

#ifndef usuario_H
#define usuario_H

#include "strUtils.h"
#include <stdio_ext.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include<unistd.h>    //write
#include <pthread.h>

// Mutex para trancar acesso a lista.
pthread_mutex_t lista;

// Ponterio para célula de login.
typedef struct login *Link;

// Armazena o nome (apelido de usuario e seu endereço IP).
typedef struct login {
    
    int *socket;
    char *nick;     
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

// Variável utilizada para indicar término para a função servidorMenssagem
// devido a estar em uma thread separada.
int signal;

/*
 * Lista de comandos.
 *      
 *      add (adiciona usuário).
 *      remove (remove usuário).
 *      list (lista usuários).
 *      send (envia mensagem para broadcast) OBS* modo padrão.
 *      quit (sai do server)
 *      help (Mostra comandos disponíveis).
 */

// Verifica se a lista de logins esta vazia.
int listaVazia();

// Inicia o descritor da lista.
Descritor *iniciarLista();

// Retorna string "nick" de acordo com os padrões de nick exigidos.
char *criaNick();

// Retorna string "ip" no formato exigido pelo padrão do endereço.
char *criaIp();

// Verifica se nick existe na tabela de usuários e retorna o endereço de sua posição na lista.
// Caso o usuário requerido não exista na lista é retornada nulo.
Link pesquisarNick(char *nick);

// Utiliza a função de pesquisar ip para retornar o ip como string.
char *retornaIp(char *ip);   

// Insere uma nova celula na tabela de logins com seu respectivo endereço IP.
int inserirUsuario();

// Imprimi a lista de usuários ativos no servidor.
void imprimirLista();

// Remove usuário da lista do server.
int removerUsuario(char *nick);

// Utiliza socket da função escultaSolicitação para enviar mensagem.
void enviarMensagem(char *buffer, int idSocket);

// Envia o o tamanho e a string nick
void enviarStr(int idSocket, char *str);

// Recebe uma string e o seu tamanho.
int recebeStr(int idSocket, char **donoThread);

// Similar a enviar mensagem porem envia um bloco com size
// do login, login e o buffer.
void enviarBloco(char *buffer, char *login, int sock);

// recebe mensagens em blocos com size login, login e buffer.
int recebeBloco(char **buffer, char **nickEmissor, int idSocket);

// Fica escutando as conexões na porta do serviço e
// recebe as mensagem do cliente e as imprimi na tela.
// parecido com o servidor de mensagem no módulo servidor
// porém nao redireciona mensagens...
void recebeMensagem(void *idSocket);

#endif