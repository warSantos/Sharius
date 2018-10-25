/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef cliente_H
#define cliente_H

#include "../headers/usuario.h"

Jogador jogadorCliente;
Mesa mesaCliente;
/*
 * 
 * Módulo cliente responsável por enviar mensagens para o servidor.
 *  
 */

// Função ajuda para opções do modo de mensagem.
void ajudaMenssagem();

// Função ajuda para menu de comando para usuário.
void ajudaComando();

// Abre conexão inicial com o servidor...
void abreConexao();

void menuOperacao ();

void menu(int valorRodada);

void menuMensagem(char *buffer);

void menuTruco(int valorRodada);

void receberCartas ();

void jogar();

void aumentoValor();

void visualizarMesa();

void visualizarCarta();

#endif