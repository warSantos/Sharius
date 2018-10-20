#include "../headers/cliente.h"

int main(){       
                       
    // Abrindo conexão com o servidor...
    jogadorCliente.socket = abreConexao(&jogadorCliente.nick);        
    
    if(jogadorCliente.socket == -1){
        
        printf("Falha ao criar conexão com o servidor remoto...\n");
        return 1;
    }
    
    // Iniciando servidor para recebimento de mensagem.    
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) recebeMensagem, (void *) &jogadorCliente.socket)){    
        
        printf("Erro na inicialiazação do servidor de escuta...\n");
        return 1;
    }
    
    // menu de administração do cliente.
    menuOperacao(jogadorCliente.nick, jogadorCliente.socket);
    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}