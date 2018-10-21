#ifndef truco_H
#define truco_H

#include "../headers/strUtils.h"

// Estrutura para abrigar as configurações de uma carta.
// O inteiro valor é utilizado para criar hierarquia entre
// as cartas e o valor nome armazena na posição 0 o valor
// de uma carta e a segunda posição o nipe. Ex.: ao = Az de ouros
typedef struct{
	int valor;
	char nome[3];
} Carta;

// Estrutura destinada para abrigar as configurações de um
// jogador. Esta possui um vetor de 3 cartas correspondetes 
// a mão do jogador, o inteiro numero para identificação na 
// mesa, um nickname e seu socket_descriptor.
typedef struct{
	int numero;
	int socket;
	char *nick;
	Carta mao[3];
} Jogador;

// Estrutura destinada a armazenar o estado da mesa de truco
// durante a rodada. Possui o identificador de quem ganhou a
// última rodada (numeroJogador) e possui as jogadas cartas 
// da rodada.
typedef struct{
	int numeroJogador;
	int tamMesa;
	Carta cartas[4];
} Mesa;

void construirBaralho(Carta *baralho);

void embaralhar(Carta *baralho);

void distribuirCartas(Jogador *jogadores, Carta *baralho);

int vencerRodada();

#endif