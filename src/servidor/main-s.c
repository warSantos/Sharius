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
    
    pthread_t t;
    if(pthread_create(&t, NULL, (void *) escutaSolicitacao, (void *) senha)){    
        
        printf("Erro na inicialiazação do servidor de inicialização...\n");
        return 1;
    }        
    
    // menu de administração do servidor.
    menuOperacao(senha);
    pthread_join(t, NULL);   
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);    
    
    return 0;
}