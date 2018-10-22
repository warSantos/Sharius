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

// Estrutura destinada a comportar os dados referentes a uma mensagem
// enviada a partir do protocolo tcp. Todo o texto enviado é armazenado
// no vetor *msg. O tamanho da mensagem em lenght e o retorno das funções
// read utilizadas nos processos de leitura armazenados em read_bytes.
typedef struct mensagem {

    char *msg;
    uint32_t lenght;
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

// Retorna string "ip" no formato exigido pelo padrão do endereço.
char *criaIp();

void enviarInt (int idSocket, int valor);

u_int32_t recebeInt (int idSocket);

// Envia o o tamanho e a string nick
void enviarStr(int idSocket, char *str);

// Realiza leitura de um socket definido pelo inteiro idSocket
// e retorna os dados escritos neste socket e metadados sobre 
// os dados escritos através da estrutura Mensagem.
Mensagem *recebeStr(int idSocket);

#endif