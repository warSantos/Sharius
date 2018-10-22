#include "../headers/servidor.h"

int main(){    
    
    int socketLocal;
    pthread_t servidorCheio;
    // Inicializando a variável que contabiliza o número de 
    // conexões estabelecidas.
    qtdeConexoes = 0;
    escutaSolicitacao ();
    /*
    // TO-DO: Cria thread de resposta servidor cheio.    
    if(pthread_create (&servidorCheio, NULL, limiteAtingido, NULL)){
        printf ("Falha ao criar thread: limiteAtingido.\n");
        return 1;
    }
    */
    
    // Servidor envia as cartas para os jogadores.
    while (1){
        sleep (1);
        printf ("Servidor ON.\n");
    }
    return 0;
}