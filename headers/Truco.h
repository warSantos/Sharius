typedef struct{
	char nome[3];
	int valor;
}Carta;
typedef struct{
	int numero;
	char mao[7];
	int valorCarta[3];
}Jogador;
void Cartas(Carta baralho[40]);
void embaralhar(Carta baralho[40]);
void distribuircarta(Carta baralho[40],Jogador jogador[4]);