#include "../headers/truco.h"

int main(){
	
	int l=1,i,j,k=0,m=0,n = 0,o=0;
	int placarJogo[2],placarRodada[2],valorRodada = 2;
	Carta baralho[40];
	char mesa[8];
	int valorMesa[4];
	int Player[4];
	char resposta[10];
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
				scanf("%s", resposta);
				__fpurge(stdin);
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
				printf("\n Vez do jogador %i\n",jogador[j].numero);
				for(q=0;q<3;q++){
					if(jogador[j].mao[q].nome[0] != 0){
						printf("%c%c\n",jogador[j].mao[q].nome[0],jogador[j].mao[q].nome[1] );
					}
				}
				printf("\n");
				scanf("%s",resposta);
				for(k=0;k<3;){
					if(jogador[j].mao[k].nome[0] == resposta[0] && jogador[j].mao[k].nome[1] == resposta[1]){
							strcpy(mesa[m].carta.nome,jogador[j].mao[k].nome);
							jogador[j].mao[k].nome[0] = 0;
							jogador[j].mao[k].nome[1] = 1;
							mesa[m].carta.valor = jogador[j].mao[k].valor;
							mesa[m].numero = jogador[j].numero;
							k = 3;
							m++;
					}
					if(k != 3){
						k++;
					}
				}
				o = 0;
				printf("mesa\n");
				while (o!= m){
					printf("%c%c  ",mesa[o].carta.nome[0],mesa[o].carta.nome[1]);
					o++;
				}
			}
			resultado = vencRodada(mesa);
			if(resultado == 5){
				printf("resultado da rodada foi empate\n");
			}
			else{
				printf("Vencedor da rodada foi %i\n", resultado);
			}++;

		}
			}
			i++;

		}
		if(placarJogo[0] > 10 || placarJogo[1] > 10){
			l=0;
		}
	}
}