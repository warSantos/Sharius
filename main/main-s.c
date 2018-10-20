#include "../headers/servidor.h"

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
    
    // Inicializando a variável que contabiliza o número de 
    // conexões estabelecidas.
    qtdeConexoes = 0;
    escutaSolicitacao (senha);

    // Servidor envia as cartas para os jogadores.
    
    return 0;
}