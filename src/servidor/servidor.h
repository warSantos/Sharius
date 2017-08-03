#ifndef menu_H
#define menu_H

#include "../headers/usuario.h"
#include <stdio_ext.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include<unistd.h>    //write

/*
 *
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 * 
 */

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
int menuComando(char *buffer);

// Função para delegar o buffer a função de estraiMenssagem para processamento
// e delegação dos dados processados para a função de enviar mesagem.
void menuMenssagem(char *buffer, int socket);

// Menu de interação do usuário hospede do servidor.
// Nesta função estão reunidas as funções de gerenciamento de 
// usuário e também o mesageiro para o hospede do server.
void menuOperacao(int socket);

// Insere os dados dos usuários via socket.
void addUserRemoto(Descritor *listaLogin, char *nick, int *sock);

// Fica aguardando conexões e envio de mesagens.
// Responsável por repassar as mesagens para os demais usuários,
// Receber solicitações de login.
// Gerenciamento de conexões externas.
void escutaSolicitacao();

// Recebe as mensagens de um cliente e repassa elas aos demais 
// logados no chat.
void *escutaCliente(void *idSocket);

// Função cliente similar a do módulo cliente utilizada para realizar conexão local
// para que o adm do servidor também possa enviar e receber mesagens.
void cliente();

// Abre conexão local com o servidor...
int abreConexao();

// Similar a enviar mensagem porem envia um bloco com size
// do login, login e o buffer.
void enviarBloco(char *buffer, char *login, int sock);

// envia mensagem através de um socket aberto.
void enviarMensagem(char *buffer, int socket);

#endif