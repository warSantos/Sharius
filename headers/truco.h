#ifndef truco_H
#define truco_H

#include "../headers/strUtils.h"

typedef struct{
	int valor;
	char nome[3];
} Carta;

typedef struct{
	int numero;
	int socket;
	char *nick;
	Carta mao[3];
} Jogador;

typedef struct{
	int numeroJogador;
	Carta cartas[4];
} Mesa;

void construirBaralho();

void embaralhar();

void distribuircarta();

int vencRodada();

#endif