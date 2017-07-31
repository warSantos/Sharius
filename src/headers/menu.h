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
// e delegação dos dados processados para a função de enviar menssagem.
void menuMenssagem(char *buffer);

// Menu de interação do usuário hospede do servidor.
// Nesta função estão reunidas as funções de gerenciamento de 
// usuário e também o menssageiro para o hospede do server.
void menuOperacao();

// Fica aguardando conexões e envio de menssagens.
// Responsável por repassar as menssagens para os demais usuários,
// Receber solicitações de login.
// Gerenciamento de conexões externas.
void servidorMenssagem();

// cria conexão para envio da mensagem.
void *enviarMenssagem();

#endif