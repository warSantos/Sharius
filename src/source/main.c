#include "../headers/menu.h"

int main(){
    
    pthread_t t;
    pthread_create(&t, NULL, (void *) servidorMenssagem, NULL);    
    pthread_join(t, NULL);
    menuOperacao();
    
    return 0;
}