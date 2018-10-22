#include "../headers/usuario.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* FUNÇÕES PARA TRABALHO DE GERENCIAMENTO DE USUÁRIO! */

char *criaIp(){
    
    //char *ip = (char *) malloc(sizeof(char) * 16);    
    char *ip = "127.0.0.1";
    while (1) {
        
        printf("Endereços IP devem estar da seguinte forma: 192.168.0.100 .\n");
        printf("Digite o endereço: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if (!verificaIp(ip)) {

            return ip;            
        }

        char op;
        while (1) {

            printf("Deseja reinserir o ip (S/n): ");
            scanf("%c", &op);
            __fpurge(stdin);
            if (op == 'S' || op == 's') {

                break;
            } else if (op == 'N' || op == 'n') {

                return NULL;
            }
        }                
    }
}

/* FUNÇÕES PARA TRABALHO COM SOCKETS! */

void enviarStr(int idSocket, char *str){
    
    int len = strlen(str) + 1;
    char *lenght = retChar(len);
    
    // enviando o tamanho da string.
    write(idSocket, lenght, 4);
    
    // enviando a string.
    write(idSocket, str, retInt(lenght));    
}

Mensagem *recebeStr(int idSocket){
        
    Mensagem *msg = malloc (sizeof(Mensagem));
    char *lenght = malloc(4);
    
    // recebendo o tamanho da string.
    msg->bytes_read = recv(idSocket, lenght, 4, 0);
    if(msg->bytes_read <= 0){

        msg->msg = NULL;
        msg->lenght = 0;       
        return msg;
    }    
    msg->lenght = retInt(lenght);
    // recebendo a string.
    msg->msg = malloc(msg->lenght);
    msg->bytes_read = recv(idSocket, msg->msg, msg->lenght, 0);
    free (lenght);
    return msg;
}