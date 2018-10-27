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

// Abre conexão inicial com o servidor e realizado todo processo de
// autenticação, inserção de jogador em duplas e estabelecimento de
// conexão.
void abreConexao (void);

// Coordena o fluxo do jogo no cliente e decodifica as mensagens pro-
// -vindas do servidor direcionando as jogadas do cliente.
void decodificador (void);

// 
void menu (int valorRodada, int bloqueioAumento);

void menuMensagem (int bloqueioAumento);

void menuTruco (int valorRodada);

void receberCartas (void);

void jogar (void);

void aumentoValor (void);

void visualizarMesa (void);

void visualizarCarta (void);

#endif