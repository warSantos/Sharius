#include "../headers/servidor.h"

int main(){
    
    // Inicializando a variável que contabiliza o número de 
    // conexões estabelecidas.
    qtdeConexoes = 0;
    escutaSolicitacao ();
    
    controleJogo ();
    
    return 0;
}