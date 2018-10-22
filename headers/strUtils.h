#ifndef strUtils_H
#define strUtils_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct comando {

    char *comando;
    char *parametro;
    int lenghtComando;
    int lenghtParametro;
} Comando;

#define stage(M) printf ("Passo: %d", M);

// faz casting de size_t para char.
char *retChar(int t);

// copia o valor de um vetor de char para um inteiro.
int retInt(char *t);

// Verifica recebe uma string e verica se ela possui algum caractere proibido.
// Os caracteres proibidos devem ser passados na string caracOut.
int charInvalido(char *str, char *caracteresInvalidos);

// Verifica se o endereço inserido esta dentro do padrão de endereço.
int verificaIp(char *ip);

// Recebe buffer de entrada do usuário e transforma em um comando compreensível.
// Procura primeiro espaço depois de um caracter e identifica comando.
Comando *split(char *buffer);

// Trabalha os dados do buffer identificando os tipos de mensagem (privada)
// ou broacast.
Comando *extraiMensagem(char *buffer);

#endif