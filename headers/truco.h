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
#define QTDE_JOGADORES 1

void construirBaralho (Carta *baralho);

void embaralhar (Carta *baralho);

void distribuirCartas (Jogador *jogadores, Carta *baralho);

int vencerTurno (Mesa *mesa);

int vencerRodada (int *placarRodada,int primeiroTurno,int turno);

#endif