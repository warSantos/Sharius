#include "../servidor/servidor.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    listaLogin = iniciarLista();
    pthread_mutex_init(&lista, NULL);
    
    // Cria conexão inicial com clientes e fornece uma thread de
    // recebimento de menagem para o cliente.
    //char *senha = malloc(sizeof(char)*16);
    char *senha = "123";
    //printf("Digite a senha de gerenciamento: ");
    //scanf("%15[^\n]s", senha);
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) escutaSolicitacao, (void *) senha)){    
        
        printf("Erro na inicialiazação do servidor de mensagem...\n");
        return 1;
    }
    
    // Esperando a thread de escuta iniciar...
    usleep(500);
    /*
    // Abrindo conexão local...
    int idSocket = abreConexao(); 
    
    if(idSocket == -1){
        
        printf("Erro ao abrir conexão com servidor local...\n");
        return 1;
    }
    */
    int idSocket = 0;
    // menu de administração do servidor.
    menuOperacao(idSocket);
    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}