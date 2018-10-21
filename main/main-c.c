#include "../headers/cliente.h"

int main(){       
                       
    // Abrindo conexão com o servidor.
    abreConexao();
    
    if(jogadorCliente.socket == -1){
        
        printf("Falha ao criar conexão com o servidor.\n");
        return 1;
    }      
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) recebeMensagem, (void *) &jogadorCliente.socket)){    
        
        printf("Erro na inicialiazação do servidor de escuta...\n");
        return 1;
    }
    
    //TO-DO: Chamar função de operação.

    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}