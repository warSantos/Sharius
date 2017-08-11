#include "../servidor/servidor.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    listaLogin = iniciarLista();
    pthread_mutex_init(&lista, NULL);
    
    // Cria conexão inicial com clientes e fornece uma thread de
    // recebimento de menagem para o cliente.
    char *senha = malloc(sizeof(char)*16);    
    printf("Digite a senha de gerenciamento: ");
    scanf("%15[^\n]s", senha);
    __fpurge(stdin);
    
    pthread_t t, t1;
    if(pthread_create(&t, NULL, (void *) escutaSolicitacao, (void *) senha)){    
        
        printf("Erro na inicialiazação do servidor de mensagem...\n");
        return 1;
    }
    
    // Esperando a thread de escuta iniciar...
    usleep(500);
    printf("servidor de escuta inicializado...\n\n");
    // Abrindo conexão local...
    char *userNick;
    
    int idSocket = abreConexaoLocal(&userNick, senha); 
    
    if(idSocket == -1){
        
        printf("Erro ao abrir conexão com servidor local...\n");
        return 1;
    }   
    
    // abrindo thread de escuta do servidor...
    if(pthread_create(&t1, NULL, (void *) recebeMensagem, (void *) &idSocket)){    
        
        printf("Erro na inicialiazação do servidor de escuta...\n");
        return 1;
    }
    
    // menu de administração do servidor.
    menuOperacao(idSocket);
    pthread_join(t, NULL);
    pthread_join(t1, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    close(idSocket);
    
    return 0;
}