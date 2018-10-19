#ifndef truco_H
#define truco_H

#include "../headers/strUtils.h"

typedef struct{
	int valor;
	char nome[3];
} Carta;

typedef struct{
	int numero;
	Carta mao[3];
} Jogador;

typedef struct{
	int numero;
	Carta carta;
} Mesa;

void Cartas(Carta baralho[40]);

void embaralhar(Carta baralho[40]);

void distribuircarta(Carta baralho[40],Jogador jogador[4]);

int vencRodada(Mesa mesa[4]);

#endif