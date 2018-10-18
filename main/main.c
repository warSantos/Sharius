#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "Truco.h"
int main(){
	int l=1,i,j,k=0,m=0,n = 0,o=0;
	int placarJogo[2],placarRodada[2],valorRodada = 2;
	Carta baralho[40];
	char mesa[8];
	int valorMesa[4];
	int Player[4];
	char resposta[10];
	Jogador jogador[4];
	//enquanto n acabar o jogo
	Cartas(baralho);
	for(i=0;i<4;i++){
		jogador[i].numero = i+1;
	}
	for(i=0;i<40;i++){
		printf("carta :%c%c\n",baralho[i].nome[0],baralho[i].nome[1]);
		printf("valor :%i\n",baralho[i].valor );
	}
	embaralhar(baralho);
	printf("Embaralhado\n");
	for(i=0;i<40;i++){
		printf("carta :%c%c\n",baralho[i].nome[0],baralho[i].nome[1]);
		printf("valor :%i\n",baralho[i].valor );
	}
	distribuircarta(baralho,jogador);
	printf("distribuircarta\n");
	for(i=0;i<4;i++){
		printf("\n%i\n",jogador[i].numero);
		printf("%c%c\n",jogador[i].mao[0],jogador[i].mao[1] );
		printf("%i\n",jogador[i].valorCarta[0]);
		printf("%c%c\n",jogador[i].mao[2],jogador[i].mao[3] );
		printf("%i\n",jogador[i].valorCarta[1]);
		printf("%c%c\n",jogador[i].mao[4],jogador[i].mao[5] );
		printf("%i\n",jogador[i].valorCarta[2]);
	}
	while(l!=0){
		Cartas(baralho);
		embaralhar(baralho);
		distribuircarta(baralho,jogador);
		i=0;
		if(placarJogo[0] == 10 || placarJogo[1] == 10){
			if(placarJogo[0] == 10){
				printf("Mao de 10 do time 1 , jogador 1 e 3 \n");
				scanf("%s",resposta);
				if(strcmp(resposta,"sim") == 0){
					valorRodada = 4;
				}
				else{
					i = 3;
				}
			}else if(placarJogo[1] == 10){
				printf("Mao de 10 do time 2 , jogador 2 e 4 \n");
				scanf("%s",resposta);
				if(strcmp(resposta,"sim") == 0){
				valorRodada = 4;
				}
				else{
				i = 3;
			}
			}
			
		}
		while(i!=3){
			for(j=0;j<4;j++){
				printf("Vez do jogador %i\n",jogador[j].numero);
				scanf("%s",resposta);
				if(strcmp(resposta,"truco") == 0 && valorRodada == 2){
					truco();

				}
				if(strcmp(resposta,"seis") == 0 && valorRodada == 4){
					seis();
				}
				if(strcmp(resposta,"nove") == 0 && valorRodada == 6){
					nove();
				}
				if(strcmp(resposta,"doze") == 0 && valorRodada == 10){
					doze();
				}
				for(k=0;k<7;){
					if(jogador[j].mao[k] == resposta[0]){
						if(jogador[j].mao[k+1]== resposta[1]){
							mesa[m] = jogador[j].mao[k];
							jogador[j].mao[k] = 0;
							m++;
							mesa[m] = jogador[j].mao[k+1];
							jogador[j].mao[k+1] = 0;
							m++;
							if(k == 0){
								valorMesa[n] = jogador[j].valorCarta[0];
								n++;
							}
							else if(k == 2){
								valorMesa[n] = jogador[j].valorCarta[1];
								n++;
							}
							else{
								valorMesa[n] = jogador[j].valorCarta[2];
								n++;
							}
							k = 7;
						}
					}
					if(k != 7){
						k++;
					}
				}
				o = 0;
				printf("mesa\n");
				while (o!= m){
					printf("%c%c\n",mesa[o],mesa[o+1] );
					o = o+2;
				}
			}
			i++;

		}
		if(placarJogo[0] > 10 || placarJogo[1] > 10){
			l=0;
		}
	}
}