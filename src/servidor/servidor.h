#ifndef menu_H
#define menu_H

#include "../headers/usuario.h"

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

// Similar a enviar mensagem porem envia um bloco com size
// do login, login e o buffer.
void enviarBloco(char *buffer, char *login, int sock);

// envia mensagem através de um socket aberto.
void enviarMensagem(char *buffer, int socket);

// Abre conexão local...
int abreConexaoLocal(char **userNick, char *senha);

// Encerra os socket quando o servidor é terminado...
void fechaConexoes();

#endif