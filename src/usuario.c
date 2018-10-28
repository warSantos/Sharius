#include "../headers/usuario.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* FUNÇÕES PARA TRABALHO DE GERENCIAMENTO DE USUÁRIO! */

char *criaIp (void){
    
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

void enviarInt (int idSocket, int valor){

    // convertendo o valor do inteiro para rede.
    u_int32_t v = htonl (valor);
    int bytes_write = write (idSocket, &v, sizeof(u_int32_t));
    if (bytes_write == 0){
        printf ("Erro: nenhum byte foi escritno no socket.\n");
    }else if (bytes_write < 0){
        printf ("Erro: falha ao escrever inteiro no socket.\n");
    }
}

u_int32_t recebeInt (int idSocket){

    u_int32_t v;
    int bytes_read = recv(idSocket, &v, sizeof(u_int32_t), 0);
    // Se nenhum byte for lido (cliente fez uma interrupção de teclado por exemplo).
    if (bytes_read < 0){
        printf ("Erro: falha ao ler inteiro no socket.\n");
        return -1;
    }
    return ntohl (v);
}

void enviarStr (int idSocket, char *str){

    // Obtendo o tamanho da string.
    u_int32_t lenght = strlen(str) + 1;

    // enviando o tamanho da string.
    enviarInt (idSocket, lenght);
    
    // enviando a string.
    int bytes_write = write(idSocket, str, lenght);
    if (bytes_write == 0){
        printf ("Erro: nenhum byte foi escritno no socket.\n");
    }else if (bytes_write < 0){
        printf ("Erro: falha ao escrever string no socket.\n");
    }
}

Mensagem *recebeStr (int idSocket){
        
    Mensagem *msg = malloc (sizeof(Mensagem));
    
    // recebendo o tamanho da string.
    msg->lenght = recebeInt (idSocket);
    if(msg->bytes_read < 0){
        printf ("Erro: erro ao ler string.\n");
        msg->msg = NULL;
        msg->lenght = 0;
        return msg;
    }
    // recebendo a string.
    msg->msg = malloc(msg->lenght);
    msg->bytes_read = recv(idSocket, msg->msg, msg->lenght, 0);
    return msg;
}