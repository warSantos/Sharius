#include "../headers/cliente.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void ajudaMensagem(){
        
    printf("\n\tAjuda.\n\n");
    printf("00 - jogar Carta , habilita voce joga sua carta na mesa\n");  
    printf("01 - truco\n");
}

void ajudaComando(){

    printf("\n\tAjuda.\n\n");
    printf("\n\t!m altera para o modo de mensagem.\n\n");        
    printf("\n\tclear - limpa o bufferscreen (tela).");
    printf("\n\tlist - imprimi lista de usuários ativos.");
    printf("\n\thelp - consulta ajuda.");
    printf("\n\tquit - encerra conexão com logins e deleta sala.\n");        
}

void abreConexao(){
        
    struct sockaddr_in servidor;
    //Create socket
    jogadorCliente.socket = socket(AF_INET , SOCK_STREAM , 0);
    if (jogadorCliente.socket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return;
    }   
    /*
     * Reutilização da estrutura mensagem para retornar valor
     * do endereço ip ja informado.
    */
    Mensagem *msg = malloc (sizeof(Mensagem));
    /*msg->msg = malloc(sizeof(char)*16);
    while(1){
        
        printf("Digite o ip do servidor: ");
        scanf("%15[^\n]s", msg->msg);
        __fpurge(stdin);
        if(!verificaIp(msg->msg)){
            
            break;
        }
    }*/
    msg->msg = "127.0.0.1";
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr(msg->msg);
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(40001);
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor.
    if (connect(jogadorCliente.socket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        //return NULL;
        return;
    }
    // Recebe mensagem de autenticação.
    int tentativas = 0;
    char ok;
    while(tentativas < 3){                
        /*
        //Recebendo senha.
        char *resposta = malloc(sizeof(char)*16);
        printf("Senha de acesso: ");
        
        scanf("%15[^\n]s", resposta);
        __fpurge(stdin);                                
        */
        // enviando senha.
        char *resposta = "123";
        enviarStr(jogadorCliente.socket, resposta);
        // Recebendo confiramção.
        // TO-DO: padronizar tudo para (read, write) ou (send, recv).
        // TO-DO: utilizar somente recebeStr e enviarStr.
        recv(jogadorCliente.socket, &ok, sizeof (char), 0);
        
        // Se o acesso for permitido receba a nova porta para o 
        // onde o server fara o fluxo do jogo.
        if(ok == 'S'){
            
            break;
        }        
        printf("Senha errada.\n");
        tentativas++;
    }
    if(tentativas == 3){
        
        printf("Conexão perdida.\n\n");
        close(jogadorCliente.socket);
        return;
    }
    printf("Por favor aguarde o inicio da partida.\n");
    //return msg;
}

void menu(int valorRodada){
    
    printf("00 - Jogar Carta\n");
    if(valorRodada == 2){
        printf("01 - Pedir truco\n");
    }else if (valorRodada == 4){
        printf("01 - Pedir seis\n");
    }else if(valorRodada == 8){
        printf("01 - Pedir nove\n");
    }else if(valorRodada == 10){
        printf("01 - Pedir Jogo\n");
    }

}

//opções jogador pode fazer
void menuMensagem(char *buffer, int valorRodada){
    
    while (1){
        // jogar a carta comando 00
        if(!strncmp(buffer,"00" , 3)){
            enviarStr(jogadorCliente.socket, "00");
            jogar();
            return;
        }// Se o jogador for aumentar a aposta.
        else if(!strncmp(buffer, "01", 3)){
            enviarStr(jogadorCliente.socket, "01");
            return;
        }else{
            printf("Opção inválida, por favor digite novamente.\n");            
        }
    }
}

//menuTruco se caso o jogar for trucado caira aki
void menuTruco(int valorRodada){
    
    char resposta[3];
    // Enquanto o usuário não digitar uma opção válida.
    while (1){
        
        if(valorRodada == 2){
            printf("01 - Aumentar para seis\n");
        }else if(valorRodada == 4){
            printf("01 - Aumentar para nove\n");
        }else if(valorRodada == 8){
            printf("01 - Aumentar para doze\n");
        }
        printf("02 - Aceitar a aposta\n");
        printf("03 - Recusar a aposta\n");
        scanf("%2[\n^]s",resposta);
        __fpurge(stdin);
        if(!strncmp(resposta,"01", 3)){
            // Enviando aumento de aposta.
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }else if(!strncmp(resposta, "02", 3)){
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }
        else if(!strncmp(resposta, "03", 3)){
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }else{
            printf ("Opção inválida.\n");
        }
    }
}

void menuOperacao (){
    
    char buffer[3];
    Mensagem *msg;
    int valorRodada;
    // Recebendo mensagem inicial de inicio de partida.
    msg = recebeStr(jogadorCliente.socket);
    if(msg->bytes_read < 0){
        printf("Erro: falha ao receber a mensagem.\n");
        return;
    }
    printf (msg->msg);
    free(msg);
    while(1){
        // Setando o valor da rodada para o valor inicial.
        valorRodada = 2;
        sleep(1);
        msg = recebeStr (jogadorCliente.socket);
        printf ("sinal: %s\n", msg->msg);
        // Se for uma solicitação de aumento de aposta.
        if(!strncmp(msg->msg, "01", 3)){
            menuTruco(valorRodada);
        }// Se for um sinal de permissão para jogar.
        else if(!strncmp(msg->msg, "10",3)){
            printf ("Sua vez de jogar.\n");
            // Mostrando quais opções o jogador pode fazer.
            menu(valorRodada);
            scanf("%3[^\n]s", buffer);
            __fpurge(stdin);
            menuMensagem(buffer, valorRodada);
        }// Se for um sinal de envio de mesa.
        else if (!strncmp(msg->msg, "11", 3)){
            // Recebendo as cartas na mesa do servidor.
            visualizarMesa ();
        }else if (!strncmp(msg->msg, "12", 3)){
            // Receba as cartas do servidor.
            printf ("Recebendo cartas.\n");
            receberCartas ();
            visualizarCarta ();
        }else{
            printf("Vez de outro jogador, espere sua vez\n");
        }
    }
    close(jogadorCliente.socket);
}

void receberCartas (){

    int numeroCarta;
    Mensagem *msg;
    for (numeroCarta = 0; numeroCarta < 3; numeroCarta++){
        
        msg = recebeStr (jogadorCliente.socket);
        // Recebendo as cartas do jogador (carta por carta).
        if (msg->bytes_read < 0){
            printf ("Erro: falha ao receber as cartas.\n");
            return;
        }
        memcpy (jogadorCliente.mao[numeroCarta].nome, msg->msg, msg->lenght);
        // Recebendo o valor da carta.
        jogadorCliente.mao[numeroCarta].valor = recebeInt (jogadorCliente.socket);
    }
}

void jogar(){

    while (1){ // Enquanto o jogador não jogar um carta certa.
        
        visualizarCarta ();
        printf("Escolha a carta que voce vai jogar: ");
        char resposta[3];
        scanf("%2[^\n]s", resposta);
        __fpurge(stdin);
        int k;
        // loop de comparar a carta que o player escolheu com a mao
        for(k = 0;k < 3; k++){
            // if que compara se tem esta carta na mao
            if(jogadorCliente.mao[k].nome[0] == resposta[0] 
                && jogadorCliente.mao[k].nome[1] == resposta[1]){            
                enviarStr(jogadorCliente.socket, jogadorCliente.mao[k].nome);
                jogadorCliente.mao[k].nome[0] = 0;
                jogadorCliente.mao[k].nome[1] = 0;
                enviarInt(jogadorCliente.socket, jogadorCliente.mao[k].valor);
                return;
                //TO-DO: Aqui deveria ser um return eu acredito.
            }
        }
        printf("Voce nao tem esta carta\n");
        // se o cara nao tem a carta a função é chamada denovo 
    }
    //TO-DO: Aqui deveria informar ao usuário que a carta não existe na mão dele.
}

void visualizarCarta(){
    
    int i;
    printf("Cartas\n");
    for(i = 0; i < 3; i++){
        if(jogadorCliente.mao[i].nome[0] != 0){
            printf("%s\t",jogadorCliente.mao[i].nome);
        }
    }
    printf("\n");
}

void visualizarMesa(){
    
    int i, tamanhoMesa;
    Mensagem *msg;
    printf("Mesa: \n");
    tamanhoMesa = recebeInt (jogadorCliente.socket);
    for(i = 0; i < tamanhoMesa; i++){
        msg = recebeStr(jogadorCliente.socket);
        printf ("%s\t", msg->msg);
        free (msg);
    }
    printf("\n");
}