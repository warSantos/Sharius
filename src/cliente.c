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

void menu(int valorRodada){
    
    printf(" 00 - Jogar Carta ");
    if(valorRodada == 2){
        printf("02 - Pedir truco\n");
    }
    if (valorRodada == 4){
        printf("03 - Pedir seis\n");
    }
    if(valorRodada == 8){
        printf("04 - Pedir nove\n");
    }
    if(valorRodada == 10){
        printf("05 - Pedir Jogo\n");
    }

}
//opções jogador pode fazer
void menuMensagem(char *buffer, int valorRodada){
    
    
    // jogar a carta comando 00
    if(!strncmp(buffer,"00" , 3)){
        jogar();
    }
    //pedir truco 01
    else if(!strncmp(buffer, "01", 3) && valorRodada == 2 ){
        enviarStr(jogadorCliente.socket,(char *) &"01");
        return;
    // pedir seis 02
    }else if(!strncmp(buffer, "02", 3) && valorRodada == 4){
        enviarStr(jogadorCliente.socket,(char *) &"02");
        return;
    // pedir nove 03
    }else if(!strncmp(buffer,"03", 3) && valorRodada == 8){
        enviarStr(jogadorCliente.socket,(char *) &"03");
        return;
    //pedir doze 04
    }else if(!strncmp(buffer,"04",3) && valorRodada == 10 ){
        enviarStr(jogadorCliente.socket,(char *) &"04");
        return;
    //recusar aposta
    }else if(!strncmp(buffer,"05",3)){
        enviarStr(jogadorCliente.socket,(char *) &"05");
        return;
    }else{
        printf("Opção inválida, por favor digite novamente.\n");
        menuMensagem(buffer,valorRodada);
    }
    enviarStr(jogadorCliente.socket, buffer);
}
//menuTruco se caso o jogar for trucado caira aki
void menuTruco(char *comando){
    char *resposta;
    resposta = calloc(sizeof(char),10);
    if(!strncmp(comando,"01",3)){
        printf("02 - Aumentar para seis\n");
    }
    else if(!strncmp(comando,"02",3)){
        printf("03 - Aumentar para nove\n");
    }
    else if(!strncmp(comando,"03",3)){
        printf("04 - Aumentar para doze\n");
    }
    printf("05 - Recusar a aposta\n");
    printf("06 - Aceitar a aposta\n");
    scanf("%s",resposta);
    __fpurge(stdin);
    if(!strncmp(resposta,"02",3)){
        enviarStr(jogadorCliente.socket,(char *) &"02");
        return;
    }
    else if(!strncmp(resposta,"03",3)){
        enviarStr(jogadorCliente.socket,(char *) &"03");
        return;
    }
    else if(!strncmp(resposta,"04",3)){
        enviarStr(jogadorCliente.socket,(char *) &"04");
        return;
    }
    else if(!strncmp(resposta,"05",3)){
        enviarStr(jogadorCliente.socket,(char *) &"05");
        return;
    }
    else if(!strncmp(resposta,"06",3)){
        enviarStr(jogadorCliente.socket,(char *) &"06");
        return;
    }

}

void menuOperacao(int valorRodada){
    
    char *buffer,*comando;
    int loop = 1;
    
    while(loop){
        sleep(10);
        receberMesa();
        visualizarCarta();
        visualizarMesa();
        comando = calloc(sizeof(char),10);
        recebeMensagem(comando);
        //se foi pedido vai ser mandado para menu truco para ver se vc aceita ,recusa ou aumenta o valor do truco;
        if(!strncmp(comando,"01",3)||!strncmp(comando,"02",3) || !strncmp(comando,"03",3) || !strncmp(comando,"04",3)){
            menuTruco(comando);
        }
        else if(!strncmp(comando,"00",3)){
        menu(valorRodada);
        buffer = calloc(sizeof(char),20);
        scanf("%s",buffer);
        __fpurge(stdin);
        menuMensagem(buffer, valorRodada);
        }
        else{
            printf("Vez de outro jogador , espere sua vez\n");
        }
    }

}

void abreConexao(){
        
    struct sockaddr_in servidor;
    //Create socket
    jogadorCliente.socket = socket(AF_INET , SOCK_STREAM , 0);
    if (jogadorCliente.socket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return;
    } 
        
    char *ip = malloc(sizeof(char)*16);    
    while(1){
        
        printf("Digite o ip do servidor: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if(!verificaIp(ip)){
            
            break;
        }
    }        
    
    // Definindo IP do servidor...
    servidor.sin_addr.s_addr = inet_addr(ip);
    
    // Definindo o tipo de protocolo...
    servidor.sin_family = AF_INET;
    
    // Define a porta em que esta ativo o serviço no servidor...
    servidor.sin_port = htons(40001);
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    //Busca conexão com o servidor...
    
    if (connect(jogadorCliente.socket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return;
    }                    
    
    // Cria usuário.
    // Recebe mensagem de autenticação.
    int tentativas = 0;
    char ok;
    while(tentativas < 3){                
        
        // Recebendo senha do teclado.
        char *resposta = malloc(sizeof(char)*16);
        printf("Senha de acesso: ");       
        
        scanf("%15[^\n]s", resposta);
        __fpurge(stdin);                                
        
        // enviando senha.
        enviarStr(jogadorCliente.socket, resposta);
        
        // Recebendo confiramção.
        // TO-DO: padronizar tudo para (read, write) ou (send, recv).
        // TO-DO: utilizar somente recebeStr e enviarStr.
        recv(jogadorCliente.socket, &ok, sizeof (char), 0);
        
        if (ok == 'S') { // se receber acesso autorizado.
                         // sai do loop e vai para criação de usuário.
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
    printf("\n\n");
    // criando usuário.
    
    char *nick;   
    
    while(1){
                
        nick = criaNick();        
        
        // enviando nick.
        enviarStr(jogadorCliente.socket, nick);
        
        // recebendo confirmação...
        recv(jogadorCliente.socket, &ok, 1, 0);
        if(ok == 'S'){
            
            break;
        }
        printf("Este login já esta em uso.\n\n");
    }        
    
    // Recebendo confirmação de preparo para receber nome...
    char ac;
    recv(jogadorCliente.socket, &ac, 1, 0);
    
    // enviando login para a thread de escutaCliente.
    enviarStr(jogadorCliente.socket, nick);
    
    jogadorCliente.nick = malloc(sizeof(char)* (strlen(nick)+1));
    strncpy(jogadorCliente.nick, nick, (strlen(nick)+1));
    
    free(nick);
    printf("Login cadastrado...\n");
    return;
}

void visualizarCarta(){
    int i;
    printf("Cartas\n");
    for(i = 0; i < 3; i++){
        if(jogadorCliente.mao[i].nome[0] != 0){
            printf("%s \t",jogadorCliente.mao[i].nome);
        }
    }
    printf("\n");               
}
void visualizarMesa(){
    int i;
    printf("Mesa\n");
    for(i=0;i < mesaCliente.tamMesa;i++){
           printf("%s \t",mesaCliente.cartas[i].nome ); 
    }
    printf("\n");
}

void jogar(){
    printf("Escolha a carta que voce vai jogar\n");
    char resposta[3];
    printf("\n");
    scanf("%2[^\n]s",resposta);
    int k;
    // loop de comparar a carta que o player escolheu com a mao
    for(k = 0;k < 3; k++){
        // if que compara se tem esta carta na mao
        if(jogadorCliente.mao[k].nome[0] == resposta[0] 
            && jogadorCliente.mao[k].nome[1] == resposta[1]){            
            enviarStr(jogadorCliente.socket, jogadorCliente.mao[k].nome);
            jogadorCliente.mao[k].nome[0] = 0;
            jogadorCliente.mao[k].nome[1] = 0;
            enviarStr(jogadorCliente.socket,(char *) &jogadorCliente.mao[k].valor);
            enviarStr(jogadorCliente.socket,(char *) &jogadorCliente.numero);
            return;
            //TO-DO: Aqui deveria ser um return eu acredito.
       }
    }
    printf("Voce nao tem esta carta\n");
    // se o cara nao tem a carta a função é chamada denovo 
    jogar();
    //TO-DO: Aqui deveria informar ao usuário que a carta não existe na mão dele.
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
        memcpy (jogadorCliente.mao[numeroCarta].nome, msg->msg, 3);        
        
        // Recebendo o valor da carta.
        msg = recebeStr (jogadorCliente.socket);
        if (msg->bytes_read < 0){
            printf ("Erro: falha ao receber as cartas.\n");
            return;
        }
        memcpy (&jogadorCliente.mao[numeroCarta].valor, msg->msg, msg->lenght);
    }
    //dei free por via da duvidas
    free(msg);
}

void jogo(){
    
    Mensagem *msg;
    // Recebendo comando do servidor.
    msg = recebeStr(jogadorCliente.socket);
    if(msg->bytes_read < 0){
        printf("Erro:falha ao receber a mensagem.\n");
        return;
    }
    //memcpy(&comando,msg->msg,msg->lenght);
    free(msg);
    // Enquanto não acabar o jogo.
    // TO-DO: Refazer função de receber mensagem
    // ate o nome seria bom alterar.
    //while (1){

    //}
    
    //fechando a conexão
    //printf(ANSI_COLOR_RED "Servidor inoperante..." ANSI_COLOR_RESET " \n");
    //close(jogadorCliente.socket);        
}
void receberMesa(){

    Mensagem *msg;
    int i,tamMesa = 0;
    for (i = 0;i< 4; i++){
        msg = recebeStr(jogadorCliente.socket);
        if(msg->bytes_read < 0){
            printf("Erro:falha ao receber a mensagem.\n");
            return;
        }
        if(msg->lenght >  0 ){
            memcpy (mesaCliente.cartas[i].nome,msg->msg,msg->lenght);
            tamMesa++;
        }
        else if(msg->lenght == 0){
            mesaCliente.tamMesa = tamMesa;
            return;
        }
    }
}