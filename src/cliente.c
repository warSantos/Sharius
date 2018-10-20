#include "../headers/cliente.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void ajudaMensagem(){
        
    printf("\n\tAjuda.\n\n");
    printf("\n\t!c altera para o modo de comando.\n\n");    
    printf("\n\tPor padrão ao se conectar a uma sala suas mensagens são enviadas");
    printf("\n\tpara todos usuários onlines na sala.");
    printf("\n\n\tPara enviar mensagens em particular adicione @ antes do nick\n");
    printf("\tdo receptor.");
    printf("\n\n\t> @FULANO MENSSAGEM...\n\n");    
    printf("\tTODA mesagem iniciada por @ será comprendido como uma mensagem privada.\n");
    printf("\tEntão evite iniciar mesagens com @ com destino para broadcast (todos na sala).\n");
    printf("\n\t-clear - limpa o bufferscreen (tela).");
    printf("\n\t-list - imprimi lista de usuários ativos.");    
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
        printf("01 - Pedir truco\n");
    }
    if (valorRodada == 4){
        printf("02 - Pedir seis\n");
    }
    if(valorRodada == 8){
        printf("03 - Pedir nove\n");
    }
    if(valorRodada == 10){
        printf("04 - Pedir Jogo\n");
    }
    printf("10 - help\n");

}

void menuMensagem(char *buffer, int valorRodada){
    
    // TO-DO verificar se a função extrair vai servir para este trabalho.
    Comando *bloco = extraiMensagem(buffer);
    if(!bloco->lenghtParametro){
        
        return;
    }
    else if(!strncpm(bloco->parametro,"00" , 3)){
        jogar();
    }
    //printf("valor de retorno %d\n", strncmp(bloco->parametro, "-help", 6));
    else if(!strncmp(bloco->parametro, "01", 3) && valorRodada == 2 ){
        aumentoValor();
        return;
    }else if(!strncmp(bloco->parametro, "02", 3 && valorRodada == 4)){
        aumentoValor();
        return;
    }else if(!strncmp(bloco->parametro,"03", 3) && valorRodada == 8){
        aumentoValor();
        return;
    }else if(!strncmp(bloco->parametro,"04",3) && valorRodada == 10 ){
        aumentoValor();
        return;
    }else if(!strncmp(bloco->parametro,"05",3)){
        
        return;
    }else if(!strncmp(bloco->parametro,"06",3)){
        return;
    }else{
        printf("Opção inválida, por favor digite novamente.\n");
        //menuOperacao
    }
    enviarStr(jogadorCliente.socket, buffer);
}

void menuOperacao007(char *userNick, int idSocket){
    char *buffer;
    menu();
    int loop = 1;
    while(loop){
        buffer = calloc(sizeof(char),20);
        scanf("%s",buffer);
        __fpurge(stdin);
        menuMensagem(buffer, jogadorCliente);
    }

}

void menuOperacao(char *userNick, int idSocket){
    
    char *buffer;
    char *alteraModo = calloc(sizeof(char),3);
    alteraModo[0] = '!';
    alteraModo[1] = 'c';
    printf("\n\n\tCarregando configurações...\n\n");
    // Menu de comandos.
    int loop = 1;
    while(loop){
        
        if(alteraModo[0] == '!' && alteraModo[1] == 'c'){
            
            printf("$ ");
        }else{
            
            printf("> ");
        }
        buffer = calloc(sizeof(char),2048);
        scanf("%2048[^\n]s", buffer);
        __fpurge(stdin);
                                
        // Identifica o princípio de um possível comando de alteração de modo.
        if (buffer[0] == '!' && buffer[1] == 'c') {
            
            system("clear");
            // Altera pro modo de comando.            
            printf("\n\nModo de comando.\n\n");
            alteraModo[0] = '!';
            alteraModo[1] = 'c';
            
            // Altera para o modo de mensagem.    
        } else if (buffer[0] == '!' && buffer[1] == 'm') {
            
            system("clear");
            printf("\n\nModo de mensagem.\n\n");
            alteraModo[0] = '!';
            alteraModo[1] = 'm';
        }        
        
        // Se estiver no mode de comandos...
        if(alteraModo[0] == '!' && alteraModo[1] == 'c'){ // chama o menu de comados.
            
            // Se o último comando utilizado nao foi o de alterar para o modo de comando...
            if(buffer[0] != '!' || buffer[1] != 'c') {
                                
                if(menuComando(buffer) == 2){
                    
                    close(idSocket);
                    exit(0);
                }                
            }                        
        }else{ // Modo de envio de mesagem...
            
            // Se o ultimo comando utilizado não foi o de alterar para o modo de mensagem...
            if(buffer[0] != '!'|| buffer[1] != 'm') {
                
                menuMensagem(buffer, userNick, idSocket);                                                               
            }
        }
        free(buffer);
    }
    free(buffer);
    free(alteraModo);    
}

int abreConexao(char **userNick){
        
    struct sockaddr_in servidor;         
    //Create socket
    int retSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (retSocket == -1){
        
        printf("Erro ao criar socket cliente...\n");
        return retSocket;
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
    
    if (connect(retSocket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
                
        perror("Erro. conexão não estabelecida...");
        return retSocket;
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
        enviarStr(retSocket, resposta);
        
        // Recebendo confiramção.
        recv(retSocket, &ok, sizeof (char), 0);
        
                
        if (ok == 'S') { // se receber acesso autorizado.
                         // sai do loop e vai para criação de usuário.
            break;
        }        
        printf("Senha errada.\n");
        tentativas++;
    }
    if(tentativas == 3){
        
        printf("Conexão perdida.\n\n");
        close(retSocket);
        return -1;
    }
    printf("\n\n");
    // criando usuário.
    
    char *nick;   
    
    while(1){
                
        nick = criaNick();        
        
        // enviando nick.
        enviarStr(retSocket, nick);
        
        // recebendo confirmação...
        recv(retSocket, &ok, 1, 0);
        if(ok == 'S'){
            
            break;
        }
        printf("Este login já esta em uso.\n\n");
    }        
    
    // Recebendo confirmação de preparo para receber nome...
    char ac;
    recv(retSocket, &ac, 1, 0);
    
    // enviando login para a thread de escutaCliente.
    enviarStr(retSocket, nick);
    
    *userNick = malloc(sizeof(char)* (strlen(nick)+1));
    strncpy(*userNick, nick, (strlen(nick)+1));
    
    free(nick);
    printf("Login cadastrado...\n");
    return retSocket;
}

void visualizarCarta(){
    int i;
    for(i = 0; i < 3; i++){
        if(jogadorCliente.mao[i].nome[0] != 0){
            printf("%s\n",jogadorCliente.mao[i].nome);
        }
    }                
}

void jogar(){

    char resposta[3];
    printf("\n");
    scanf("%s",resposta);
    int k;
    for(k = 0;k < 3; k++){
        
        if(jogadorCliente.mao[k].nome[0] == resposta[0] 
            && jogadorCliente.mao[k].nome[1] == resposta[1]){
            enviarStr(jogadorCliente.socket,jogadorCliente.mao[k].nome);
            //strcpy( ,jogadorCliente.mao[k].nome);
            jogadorCliente.mao[k].nome[0] = 0;
            jogadorCliente.mao[k].nome[1] = 0;
            enviarStr(jogadorCliente.socket,(char *) &jogadorCliente[j].mao[k].valor);
             //= jogadorCliente[j].mao[k].valor;
            // = jogadorCliente[j].numero;
            enviarStr(jogadorCliente.socket,(char *) &jogadorCliente[j].numero);
            // adicionar código de enviar carta.
            break;
        }
    }
}

void aumentoValor(){

}