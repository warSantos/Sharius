#include "../headers/servidor.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    //listaLogin = iniciarLista();
    //pthread_mutex_init(&lista, NULL);
    
    // Inicializando a variável que contabiliza o número de 
    // conexões estabelecidas.
    qtdeConexoes = 0;
    escutaSolicitacao ();

    // Servidor envia as cartas para os jogadores.
    
    return 0;
}