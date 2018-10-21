/*
 * 
 Implementação da "tabela" de login utilizando lista duplamente encadeada.
 * 
 */

#ifndef usuario_H
#define usuario_H

#include "../headers/truco.h"
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

// Estrutura destinada a comportar os dados referentes a uma mensagem
// enviada a partir do protocolo tcp. Todo o texto enviado é armazenado
// no vetor *msg. O tamanho da mensagem em lenght e o retorno das funções
// read utilizadas nos processos de leitura armazenados em read_bytes.
typedef struct mensagem {

    char *msg;
    int lenght;
    int bytes_read;
} Mensagem;


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

// Envia o o tamanho e a string nick
void enviarStr(int idSocket, char *str);

// Realiza leitura de um socket definido pelo inteiro idSocket
// e retorna os dados escritos neste socket e metadados sobre 
// os dados escritos através da estrutura Mensagem.
Mensagem *recebeStr(int idSocket);

#endif