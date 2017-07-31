#ifndef menu_H
#define menu_H

#include <pthread.h>
#include <stdio_ext.h>
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

// Fica aguardando conexões e envio de mesagens.
// Responsável por repassar as mesagens para os demais usuários,
// Receber solicitações de login.
// Gerenciamento de conexões externas.
void servidorMenssagem();

// repassa as mesagens recebidas para os clientes do chat.
void *repassarMenssagem(void *idSocket);

// Função cliente similar a do módulo cliente utilizada para realizar conexão local
// para que o adm do servidor também possa enviar e receber mesagens.
void cliente();

// Abre conexão local com o servidor...
int abreConexao();
#endif