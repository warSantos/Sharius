#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/truco.h"

void construirBaralho(Carta *baralho){
	// 4
	strcpy(baralho[0].nome, "4o"); 
	baralho[0].valor = 1;
	strcpy(baralho[1].nome, "4c"); 
	baralho[1].valor = 1;
	strcpy(baralho[2].nome, "4e"); 
	baralho[2].valor = 1;
	// 5
	strcpy(baralho[3].nome, "5p"); 
	baralho[3].valor = 2; 
	strcpy(baralho[4].nome, "5o"); 
	baralho[4].valor = 2; 
	strcpy(baralho[5].nome, "5c"); 
	baralho[5].valor = 2; 
	strcpy(baralho[6].nome, "5e"); 
	baralho[6].valor = 2;
	// 6 
	strcpy(baralho[7].nome, "6p"); 
	baralho[7].valor = 3;
	strcpy(baralho[8].nome, "6o"); 
	baralho[8].valor = 3;
	strcpy(baralho[9].nome, "6c"); 
	baralho[9].valor = 3;
	strcpy(baralho[10].nome, "6e"); 
	baralho[10].valor = 3;
	//7
	strcpy(baralho[11].nome, "7p"); 
	baralho[11].valor = 4;
	strcpy(baralho[12].nome, "7e"); 
	baralho[12].valor = 4;
	//Q
	strcpy(baralho[13].nome, "qp"); 
	baralho[13].valor = 5;
	strcpy(baralho[14].nome, "qo"); 
	baralho[14].valor = 5;
	strcpy(baralho[15].nome, "qc"); 
	baralho[15].valor = 5;
	strcpy(baralho[16].nome, "qe"); 
	baralho[16].valor = 5;
	//J  
	strcpy(baralho[17].nome, "jp"); 
	baralho[17].valor = 6;
	strcpy(baralho[18].nome, "jo"); 
	baralho[18].valor = 6;
	strcpy(baralho[19].nome, "jc"); 
	baralho[19].valor = 6;
	strcpy(baralho[20].nome, "je"); 
	baralho[20].valor = 6;
	//k
	strcpy(baralho[21].nome, "kp"); 
	baralho[21].valor = 7;
	strcpy(baralho[22].nome, "ko"); 
	baralho[22].valor = 7;
	strcpy(baralho[23].nome, "kc"); 
	baralho[23].valor = 7;
	strcpy(baralho[24].nome, "ke"); 
	baralho[24].valor = 7;
	//A
	strcpy(baralho[25].nome, "ap"); 
	baralho[25].valor = 8;
	strcpy(baralho[26].nome, "ao"); 
	baralho[26].valor = 8;
	strcpy(baralho[27].nome, "ac");
	baralho[27].valor = 8 ;
	//2
	strcpy(baralho[28].nome, "2p"); 
	baralho[28].valor = 9;
	strcpy(baralho[29].nome, "2o"); 
	baralho[29].valor = 9;
	strcpy(baralho[30].nome, "2c"); 
	baralho[30].valor = 9;
	strcpy(baralho[31].nome, "2e"); 
	//3
	baralho[31].valor = 9;
	strcpy(baralho[32].nome, "3p"); 
	baralho[32].valor = 10;
	strcpy(baralho[33].nome, "3o"); 
	baralho[33].valor = 10;
	strcpy(baralho[34].nome, "3d"); 
	baralho[34].valor = 10;
	strcpy(baralho[35].nome, "3e"); 
	//Manilhas
	baralho[35].valor = 10;
	strcpy(baralho[36].nome, "7o"); 
	baralho[36].valor = 11;
	strcpy(baralho[37].nome, "ae"); 
	baralho[37].valor = 12;
	strcpy(baralho[38].nome, "7c"); 
	baralho[38].valor = 13;
	strcpy(baralho[39].nome, "4p"); 
	baralho[39].valor = 14;
}

void embaralhar(Carta *baralho){
	int i=0,j,k,auxValor;
	char auxNome[3];
	srand(time(NULL));
	while(i != 40){
		j = rand()%40;
		k = rand()%40;
		if(k == j){
			k = rand()%40;
		}
		strcpy(auxNome, baralho[j].nome);
		strcpy(baralho[j].nome, baralho[k].nome);
		strcpy(baralho[k].nome, auxNome);
		auxValor = baralho[j].valor;
		baralho[j].valor = baralho[k].valor;
		baralho[k].valor = auxValor;
		i++;
	}
}

void distribuirCartas(Jogador *jogadores, Carta *baralho){
	
	int i,j;
	// Para cada jogador.
	for(i = 0; i < QTDE_JOGADORES; i++){
		// Para cada carta do jogador i.
		for(j = 0; j < 3; j++){
			strcpy(jogadores[i].mao[j].nome, baralho[i*j].nome);
			jogadores[i].mao[j].valor = baralho[i*j].valor;
		}
	}
}

int vencerRodada(Mesa mesa){
	
	int aux=0,aux2,i;
	/*for(i = 0; i < 4; i++){
		
		if(mesa.cartas[i].valor > aux ){
			
			aux = mesa.cartas[i].valor;
			printf("%i\n",mesa.cartas[i].valor);
			aux2 = mesa.numeroJogador;
			printf("%i\n",mesa.numeroJogador);
		}
		else if(mesa.cartas[i].valor == aux){
			
			if((aux2 % 2) == 0 && (mesa.numeroJogador % 2) != 0 
				|| (aux2 % 2) != 0 && (mesa.numeroJogador %2) == 0){
				aux2 = 5;
			}
		}
	}*/
	return aux2;
}
int vencjogo(){

}