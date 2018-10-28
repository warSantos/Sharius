#include "../headers/cliente.h"

int main(){       

    // Abrindo conexão com o servidor.
    abreConexao();
    
    if(jogadorCliente.socket == -1){
        
        printf("Falha ao criar conexão com o servidor.\n");
        return 1;
    }
    
    // Mantendo conexão ativa e controlando o fluxo do jogo.
    decodificador ();
    
    return 0;
}