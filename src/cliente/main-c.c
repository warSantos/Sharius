#include "../cliente/cliente.h"

int main(){       
    
    // Iniciando servidor para recebimento de mensagem.
    pthread_t t;
    if(pthread_create(&t, NULL, recebeMensagem, NULL)){    
        
        printf("Erro na inicialiazação do servidor de escuta...\n");
        return 1;
    }
        
    // Abrindo conexão com o servidor...
    int socket = abreConexao();
    
    if(socket == -1){
        
        printf("Falha ao criar conexão com o servidor remoto...\n");
        return 1;
    }
    
    // menu de administração do servidor.
    menuOperacao(socket);    
    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}