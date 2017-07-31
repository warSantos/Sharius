/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef cliente_H
#define cliente_H

#include <pthread.h>
#include <stdio_ext.h>
#include "../headers/usuario.h"

/*
 * 
 * Módulo cliente responsável por enviar mensagens para o servidor.
 *  
 */

// Função ajuda para opções do modo de mensagem.
void ajudaMenssagem();

// Função ajuda para menu de comando para usuário.
void ajudaComando();

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
int menuComando(char *buffer);

// Função para delegar o buffer a função de estraiMenssagem para processamento
// e delegação dos dados processados para a função de enviar mesagem.
void menuMenssagem(char *buffer, int socket);

// Menu de interação do usuário hospede do servidor.
// Nesta função estão reunidas as funções de gerenciamento de 
// usuário e também o mesageiro para o hospede do server.
void menuOperacao(int socket);

// Abre conexão inicial com o servidor...
int abreConexao();

// Fica escutando as conexões na porta do serviço e
// recebe as mensagem do cliente e as imprimi na tela.
// parecido com o servidor de mensagem no módulo servidor
// porém nao redireciona mensagens...
void *recebeMensagem();

#endif