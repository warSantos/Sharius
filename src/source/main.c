#include "../headers/menu.h"

int main(){
    
    // Iniciando o ponterio descritor com lista de usuários no server.
    listaLogin = iniciarLista();
    
    // Liberando uma thread para função específica de escuta de conexões.
    pthread_t t;
    pthread_create(&t, NULL, (void *) servidorMenssagem, NULL);    
    pthread_join(t, NULL);
    
    // menu de administração do servidor.
    menuOperacao();
    
    // Falta melhorar a função de fechar conexões...
    pthread_exit(&t);
    
    return 0;
}