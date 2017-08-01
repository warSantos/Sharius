#include "../servidor/servidor.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    listaLogin = iniciarLista();
    
    // Cria conexão inicial com clientes e fornece uma thread de
    // recebimento de menagem para o cliente.
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) escutaSolicitacao, NULL)){    
        
        printf("Erro na inicialiazação do servidor de mensagem...\n");
        return 1;
    }
    
    // Esperando a thread de escuta iniciar...
    usleep(500);
    
    // Abrindo conexão local...
    int socket = abreConexao(); 
    printf("Socket: %d\n", socket);
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