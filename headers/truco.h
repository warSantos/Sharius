#ifndef truco_H
#define truco_H

#include "../headers/strUtils.h"

// Estrutura para abrigar as configurações de uma carta.
// O inteiro valor é utilizado para criar hierarquia entre
// as cartas e o valor nome armazena na posição 0 o valor
// de uma carta e a segunda posição o nipe. Ex.: ao = Az de ouros
typedef struct{
	u_int32_t valor;
	char nome[3];
} Carta;

// Estrutura destinada para abrigar as configurações de um
// jogador. Esta possui um vetor de 3 cartas correspondetes 
// a mão do jogador, o inteiro numero para identificação na 
// mesa, um nickname e seu socket_descriptor.
typedef struct{
	u_int32_t numero;
	int socket;
	Carta mao[3];
} Jogador;

// Estrutura destinada a armazenar o estado da mesa de truco
// durante a rodada. Possui o identificador de quem ganhou a
// última rodada (numeroJogador) e possui as jogadas cartas 
// da rodada.
typedef struct{
	u_int32_t numeroJogador[4];
	u_int32_t tamMesa;
	Carta cartas[4];
} Mesa;

// 4 players (0 a 3).
#define QTDE_JOGADORES 4

// Aconselhável ter duas duplas (2 jogadores cada).
#define JOGADORES_POR_DUPLA 2

// Configura o valor e o nome das cartas na estrutura baralho.
void construirBaralho (Carta *baralho);

// Recebe um baralho configurado e altera a posição das cartas
// de forma aleatória.
void embaralhar (Carta *baralho);

// Seleciona as primeiras cartas do baralho e distribui entre os
// jogadores.
void distribuirCartas (Jogador *jogadores, Carta *baralho);

// Recebe a mesa do jogo (com cartas dos participantes) e verifica
// se alguma das equipes ganhou o turno e retorna a informação atra-
// -vés de um valor inteiro.
int vencerTurno (Mesa *mesa);

// Verifica se alguma das duplas venceu a rodada (3 turnos) ou se 
// houve empate e retorna a informação através de um inteiro.
int vencerRodada (int *placarRodada, int primeiroTurno, int turno);

#endif