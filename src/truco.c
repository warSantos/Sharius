#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/truco.h"

void construirBaralho(Carta *baralho){
	// 4
	strncpy(baralho[0].nome, "4o", 3);
	baralho[0].valor = 1;
	strncpy(baralho[1].nome, "4c", 3); 
	baralho[1].valor = 1;
	strncpy(baralho[2].nome, "4e", 3); 
	baralho[2].valor = 1;
	// 5
	strncpy(baralho[3].nome, "5p", 3); 
	baralho[3].valor = 2; 
	strncpy(baralho[4].nome, "5o", 3); 
	baralho[4].valor = 2; 
	strncpy(baralho[5].nome, "5c", 3); 
	baralho[5].valor = 2; 
	strncpy(baralho[6].nome, "5e", 3); 
	baralho[6].valor = 2;
	// 6 
	strncpy(baralho[7].nome, "6p", 3); 
	baralho[7].valor = 3;
	strncpy(baralho[8].nome, "6o", 3); 
	baralho[8].valor = 3;
	strncpy(baralho[9].nome, "6c", 3); 
	baralho[9].valor = 3;
	strncpy(baralho[10].nome, "6e", 3); 
	baralho[10].valor = 3;
	//7
	strncpy(baralho[11].nome, "7p", 3); 
	baralho[11].valor = 4;
	strncpy(baralho[12].nome, "7e", 3); 
	baralho[12].valor = 4;
	//Q
	strncpy(baralho[13].nome, "qp", 3); 
	baralho[13].valor = 5;
	strncpy(baralho[14].nome, "qo", 3); 
	baralho[14].valor = 5;
	strncpy(baralho[15].nome, "qc", 3); 
	baralho[15].valor = 5;
	strncpy(baralho[16].nome, "qe", 3); 
	baralho[16].valor = 5;
	//J  
	strncpy(baralho[17].nome, "jp", 3); 
	baralho[17].valor = 6;
	strncpy(baralho[18].nome, "jo", 3); 
	baralho[18].valor = 6;
	strncpy(baralho[19].nome, "jc", 3); 
	baralho[19].valor = 6;
	strncpy(baralho[20].nome, "je", 3); 
	baralho[20].valor = 6;
	//k
	strncpy(baralho[21].nome, "kp", 3); 
	baralho[21].valor = 7;
	strncpy(baralho[22].nome, "ko", 3); 
	baralho[22].valor = 7;
	strncpy(baralho[23].nome, "kc", 3); 
	baralho[23].valor = 7;
	strncpy(baralho[24].nome, "ke", 3); 
	baralho[24].valor = 7;
	//A
	strncpy(baralho[25].nome, "ap", 3); 
	baralho[25].valor = 8;
	strncpy(baralho[26].nome, "ao", 3); 
	baralho[26].valor = 8;
	strncpy(baralho[27].nome, "ac", 3);
	baralho[27].valor = 8 ;
	//2
	strncpy(baralho[28].nome, "2p", 3); 
	baralho[28].valor = 9;
	strncpy(baralho[29].nome, "2o", 3); 
	baralho[29].valor = 9;
	strncpy(baralho[30].nome, "2c", 3); 
	baralho[30].valor = 9;
	strncpy(baralho[31].nome, "2e", 3); 
	//3
	baralho[31].valor = 9;
	strncpy(baralho[32].nome, "3p", 3); 
	baralho[32].valor = 10;
	strncpy(baralho[33].nome, "3o", 3); 
	baralho[33].valor = 10;
	strncpy(baralho[34].nome, "3d", 3); 
	baralho[34].valor = 10;
	strncpy(baralho[35].nome, "3e", 3); 
	//Manilhas
	baralho[35].valor = 10;
	strncpy(baralho[36].nome, "7o", 3); 
	baralho[36].valor = 11;
	strncpy(baralho[37].nome, "ae", 3); 
	baralho[37].valor = 12;
	strncpy(baralho[38].nome, "7c", 3); 
	baralho[38].valor = 13;
	strncpy(baralho[39].nome, "4p", 3);
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
		strncpy(auxNome, baralho[j].nome, 3);
		strncpy(baralho[j].nome, baralho[k].nome, 3);
		strncpy(baralho[k].nome, auxNome, 3);
		auxValor = baralho[j].valor;
		baralho[j].valor = baralho[k].valor;
		baralho[k].valor = auxValor;
		i++;
	}
}

void distribuirCartas(Jogador *jogadores, Carta *baralho){
	
	int i,j,k = 0;
	// Para cada jogador.
	for(i = 0; i <= QTDE_JOGADORES; i++){
		// Para cada carta do jogador i.
		for(j = 0; j < 3; j++){
			strncpy(jogadores[i].mao[j].nome, baralho[k].nome, 3);
			jogadores[i].mao[j].valor = baralho[k].valor;
			k++;
		}
	}
}

int vencerRodada(Mesa mesa){
	
	int aux=0,aux2,i;
	for(i = 0; i < 2; i++){
		
		if(mesa.cartas[i].valor > aux ){
			
			aux = mesa.cartas[i].valor;
			printf("%i\n",mesa.cartas[i].valor);
			aux2 = mesa.numeroJogador[i];
			printf("%i\n",mesa.numeroJogador[i]);
		}
		else if(mesa.cartas[i].valor == aux){
			
			if((aux2 % 2) == 0 && (mesa.numeroJogador[i] % 2) != 0 
				|| (aux2 % 2) != 0 && (mesa.numeroJogador[i] %2) == 0){
				aux2 = 5;
			}
		}
	}
	return aux2;
}
int vencerTurno(int placarRodada[2]){
	int aux;
	if(placarRodada[0] == 2 && placarRodada[1] < 2){
		aux = 1;
		return aux;
	}
	else if(placarRodada[0] < 2 && placarRodada[1] == 2){
		aux = 2;
		return aux;
	}
	else if(placarRodada[0] == 3 && placarRodada[1] == 2 ){
		aux = 1;
		return aux;
	}
	else if(placarRodada[0] == 2 && placarRodada[1] == 3){
		aux = 2;
		return aux;
	}
	else if(placarRodada[0] == 3 && placarRodada[1] == 3){
		aux = 3;
		return aux;
	}

}
int vencjogo(){

}