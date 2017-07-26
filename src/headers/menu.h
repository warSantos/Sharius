#ifndef menu_H
#define menu_H

#include <stdio_ext.h>
#include "../headers/usuario.h"
#include <pthread.h>

/*
 *
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 * 
 */



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

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
void menuComando(char *buffer);

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

#endif