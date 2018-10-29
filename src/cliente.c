#include "../headers/cliente.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void abreConexao (void){
        
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
    
    msg->msg = malloc(sizeof(char)*16);
    while(1){
        
        printf("Digite o ip do servidor: ");
        scanf("%15[^\n]s", msg->msg);
        __fpurge(stdin);
        if(!verificaIp(msg->msg)){
            
            break;
        }
    }

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

        //Recebendo senha.
        char *resposta = malloc(sizeof(char)*16);
        printf("Senha de acesso: ");
        
        scanf("%15[^\n]s", resposta);
        __fpurge(stdin);                                
        
        // enviando senha.
        enviarStr(jogadorCliente.socket, resposta);
        // Recebendo confiramção.
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

void menuAposta (int valorRodada, int bloqueioAumento){
    
    printf("00 - Jogar Carta\n");
    // Se o puder pedir aumento de aposta, mostre o menu de acordo.
    if (!bloqueioAumento){
        if(valorRodada == 2 ){
            printf("01 - Pedir truco\n");
        }else if (valorRodada == 4){
            printf("01 - Pedir seis\n");
        }else if(valorRodada == 8){
            printf("01 - Pedir nove\n");
        }else if(valorRodada == 10){
            printf("01 - Pedir Jogo\n");
        }
    }

}

// Menu com opções de jogada.
void menuJogadas (int bloqueioAumento){
    
    char buffer[3];    
    while (1){
        scanf("%2[^\n]s", buffer);
        __fpurge(stdin);
        // jogar uma carta comando 00.
        if(!strncmp(buffer,"00" , 3)){
            enviarStr(jogadorCliente.socket, "00");
            jogar();
            return;
        }// Se o jogador for aumentar a aposta.
        else if(!strncmp(buffer, "01", 3) && !bloqueioAumento){
            enviarStr(jogadorCliente.socket, "01");
            return;
        }else{
            printf("Opção inválida, por favor digite novamente.\n");            
        }
    }
}

//menuTruco se caso o jogar for trucado caira aki
void respostaAumentoAposta (int valorRodada){
    
    char resposta[3];
    // Aguardando permissão para jogar (10).
    Mensagem *msg = recebeStr (jogadorCliente.socket);
    // Enquanto o usuário não digitar uma opção válida.
    while (1){
        
        if(valorRodada == 2){
            printf("01 - Aumentar para seis.\n");
        }else if(valorRodada == 4){
            printf("01 - Aumentar para nove.\n");
        }else if(valorRodada == 8){
            printf("01 - Aumentar para doze.\n");
        }
        printf("02 - Aceitar a aposta.\n");
        printf("03 - Recusar a aposta.\n");
        scanf("%2[^\n]s",resposta);
        __fpurge(stdin);
        
        if(!strncmp(resposta,"01", 3)){
            // Enviando aumento de aposta.
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }else if(!strncmp(resposta, "02", 3)){
            // Enviando sinal de confirmação de aumento (aceitar).
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }
        else if(!strncmp(resposta, "03", 3)){
            // Enviando sinal de recusa de aumento de aposta.
            enviarStr(jogadorCliente.socket, resposta);
            return;
        }else{
            printf ("Opção inválida.\n");
        }
    }
}

void decodificador (void){
    
    Mensagem *msg;
    int valorRodada, valorMao10 = 0, bloqueioAumento = 0;
    // Recebendo mensagem inicial de inicio de partida.
    msg = recebeStr(jogadorCliente.socket);
    if(msg->bytes_read < 0){
        printf("Erro: falha ao receber a mensagem.\n");
        return;
    }
    printf (msg->msg);
    free(msg);
    // Setando o valor da rodada para o valor inicial.
    while(1){
        sleep(1);
        msg = recebeStr (jogadorCliente.socket);
        if (msg->msg[0] == '0'){ // Sinais do cliente.
            // Se for uma solicitação de aumento de aposta.
            if(!strncmp(msg->msg, "01", 3)){
                respostaAumentoAposta (valorRodada - valorMao10);
                bloqueioAumento = 0;
            }// Se for um sinal informando que algum jogador aceitou o aumento.
            else if (!strncmp(msg->msg, "02", 3)){
                int jogadorConfirmante = recebeInt (jogadorCliente.socket);
                printf ("Jogador: %d, aceitou o aumento da aposta.\n", jogadorConfirmante);
            }// For um sinal de anúncio de aumento de aposta.
            else if (!strncmp(msg->msg, "04", 3)){
                int jogadorSolicitante = recebeInt (jogadorCliente.socket);
                printf ("Jogador: %d, acaba de solicitar aumento de aposta.\n", jogadorSolicitante);
            }// Se for um sinal de permissão para jogar.
        }else { // Sinais do servidor. 
            if(!strncmp(msg->msg, "10",3)){
                printf ("Sua vez de jogar jogador.\n");
                // Mostrando quais opções o jogador pode fazer.
                menuAposta (valorRodada - valorMao10, bloqueioAumento);
                menuJogadas (bloqueioAumento);
            }// Se for um sinal de envio de mesa.
            else if (!strncmp(msg->msg, "11", 3)){
                // Recebendo as cartas na mesa do servidor.
                visualizarMesa ();
            }// Se for um sinal de envio de cartas.
            else if (!strncmp(msg->msg, "12", 3)){
                // Receba as cartas do servidor.
                receberCartas ();
                visualizarCarta ();        
            }// Se for o envio do novo valor da rodada.
            else if (!strncmp(msg->msg, "13", 3)){
                valorRodada = recebeInt (jogadorCliente.socket);
                printf ("Valor da rodada: %d.\n", valorRodada);
            }// Se for o anuncio da dupla que venceu a partida.
            else if (!strncmp(msg->msg, "14", 3)){            
                msg = recebeStr (jogadorCliente.socket);
                printf (msg->msg);
            } // Se for um sinal de mao de 10.
            else if (!strncmp(msg->msg, "15", 3)){
                char buffer[3];
                while (1){
                    printf ("02 - Aceitar mao de 10.\n");
                    printf ("03 - Recusar mao de 10.\n");
                    scanf ("%2[^\n]s", buffer);
                    __fpurge (stdin);
                    if (!strncmp(buffer, "02", 3)
                        || !strncmp(buffer, "03", 3)){
                        enviarStr (jogadorCliente.socket, buffer);
                        break;
                    }else {
                        printf ("Opção inválida.\n");
                    }
                }
            }// Se for um sinal de bloqueio de aumento de aposta.
            else if (!strncmp(msg->msg, "16", 3)){
                bloqueioAumento = 1;
            }// Se for um sinal de desbloqueio de aumento de aposta.
            else if (!strncmp(msg->msg, "17", 3)){
                bloqueioAumento = 0;
            }// Se for um sinal de ajuste de variável.
            else if (!strncmp(msg->msg, "18", 3)){
                valorMao10 = 2;
            }
        }
        // Se algum jogador desistir da partida.
        if (!strncmp(msg->msg, "30", 3)){
            // Recebendo o número do jogador que desistiu.
            u_int32_t jogadorDesistiu = recebeInt (jogadorCliente.socket);
            printf ("Jogador % desistiu.\n", jogadorDesistiu);
            exit(1);
        }// Partida finalizada.
        else if (!strncmp(msg->msg, "31", 3)){
            printf ("Partida finalizada.\n");
            exit(1);
        }
        free(msg);
    }
    close(jogadorCliente.socket);
}

void receberCartas (void){

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

void jogar (void){

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
                enviarStr(jogadorCliente.socket, resposta);
                jogadorCliente.mao[k].nome[0] = 0;
                jogadorCliente.mao[k].nome[1] = 0;
                enviarInt(jogadorCliente.socket, jogadorCliente.mao[k].valor);
                return;
            }
        }
        printf("Voce nao tem esta carta\n");
        // se o cara nao tem a carta a função é chamada denovo 
    }
}

void visualizarCarta (void){
    
    int i;
    printf("Suas cartas: \n");
    for(i = 0; i < 3; i++){
        if(jogadorCliente.mao[i].nome[0] != 0){
            printf("%s\t",jogadorCliente.mao[i].nome);
        }
    }
    printf("\n");
}

void visualizarMesa (void){
    
    int i;
    u_int32_t tamanhoMesa;
    Mensagem *msg;
    printf("Cartas na Mesa: \n");
    tamanhoMesa = recebeInt (jogadorCliente.socket);
    for(i = 0; i < tamanhoMesa; i++){
        msg = recebeStr(jogadorCliente.socket);
        printf ("%s\t", msg->msg);
        free (msg);
    }
    printf("\n");
}