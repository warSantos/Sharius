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
    // menu de administração do servidor.
    menuOperacao();
    pthread_join(t, NULL);
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}