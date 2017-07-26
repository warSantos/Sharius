#ifndef strUtils_H
#define strUtils_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct comando {

    char *comando;
    char *parametro;
    int lenghtComando;
    int lenghtParametro;    
} Comando;

// Verifica recebe uma string e verica se ela possui algum caractere proibido.
// Os caracteres proibidos devem ser passados na string caracOut.
int charInvalido(char *str, char *caracteresInvalidos);

// Verifica se o endereço inserido esta dentro do padrão de endereço.
int verificaIp(char *ip);

// Recebe buffer de entrada do usuário e transforma em um comando compreensível.
// Procura primeiro espaço depois de um caracter e identifica comando.
Comando *split(char *buffer);

#endif

