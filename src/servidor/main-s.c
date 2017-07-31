#include "../servidor/servidor.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    listaLogin = iniciarLista();
    
    // Liberando uma thread para função específica de escuta de conexões.
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) servidorMenssagem, NULL)){    
        
        printf("Erro na inicialiazação do servidor de mensagem...\n");
        return 1;
    }
    
    // Abrindo conexão local...
    int socket = abreConexao(); 
    if(socket == -1){
        
        printf("Erro ao abrir conexão com servidor local...\n");
        return 1;
    }
    // menu de administração do servidor.
    menuOperacao(socket);
    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}