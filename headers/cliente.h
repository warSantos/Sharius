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

// Identifica qual opção de aumento de aposta esta disponível para o
// o jogador baseado no valor do jogo no momento e se o jogador foi
// o último a solicitar aumento de aposta ou não.
void menuAposta (int valorRodada, int bloqueioAumento);

// Informa ao jogador quais movimentos este pode realizar e também
// trata os dados inseridos pelo mesmo.
void menuJogadas (int bloqueioAumento);

// Informa ao jogador quais movimentos este pode realizar ao ser
// intimado em um aumento de aposta.
void respostaAumentoAposta (int valorRodada);

// Recebe as cartas enviadas para o jogador pelo server e as armazena
// na estrutura jogadorCliente.
void receberCartas (void);

// Verifica quais cartas o jogador possui e oferece interface necessária
// para que o jogador possa escolher uma carta e enviar a mesma ao servidor.
void jogar (void);

// Ao servidor enviar as cartas existentes na mesa para os jogadores esta
// função recebe o fluxo de dados e trata os de foma que a visualização 
// das cartas fique mais harmoniosa.
void visualizarMesa (void);

// Ao receber as cartas do servidor esta função imprime as cartas de forma
// mais harmoniosa na tela.
void visualizarCarta (void);

#endif