#include "../headers/servidor.h"

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
    printf("\n\tadd - adiciona logins a sala.");
    printf("\n\tclear - limpa o bufferscreen (tela).");
    printf("\n\tlist - imprimi lista de usuários ativos.");
    printf("\n\thelp - consulta ajuda.");
    printf("\n\tquit - encerra conexão com logins e deleta sala.");
    
    printf("\n\tremove - remove um usuário.\n\n");    
}

void addUserRemoto(char *nick, int *sock){
    
    size_t len = strlen(nick)+1;
    Link aux = malloc(sizeof(Login));
    aux->nick = malloc(sizeof(char) * len);
    aux->socket = sock;
    strncpy(aux->nick, nick, len);
    // Caso seja a primeira inserção de um usuário na lista.
    if(listaVazia()){
        
        listaLogin->primeiro = aux;
        listaLogin->ultimo = aux;
        aux->prox = NULL;          
    }else{
    
        listaLogin->ultimo->prox = aux;
        listaLogin->ultimo = aux;
    }
    listaLogin->tamanho++;    
}

void escutaSolicitacao(char *senha){
        
    int socketLocal, socketCliente, sizeSockaddr, *novoSocket;
    struct sockaddr_in servidor, cliente;
    
    // Criando um socket para receber fluxo tcp de dados.
    // AF_INET tipo de conexão sobre IP para redes.
    // SOCK_STREAM protocolo com controle de erros.
    // 0 seleção do protocolo TCP
    socketLocal = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socketLocal == -1){
        
        printf("Erro ao criar socket local.\n");
        return;
    }
        
    servidor.sin_family = AF_INET; // Atribuindo a familia de protocolos para Internet
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(40001); // Setando e porta em que rodara o processo.       
    
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    // Criando link entre a estrutura servidor ao ID do socketLocal.
    if(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro no bind.\n");
        fechaConexoes();
        close(socketLocal);       
        exit(1);
    }    
    // Limitando o número de conexões que o servidor vai aceitar para 4 conexões.
    listen(socketLocal, 4);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    // Esperando por conexões.
    
    while(1){
                
        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        //TO-DO: tratar melhor a possibilidade de erro neste caso.
        if(socketCliente < 0){
            
            break;
        }
        // Se o limite de conexões não foi atingido.
        if (qtdeConexoes < 4){

            // Criando uma thread para um cliente.
            // A nova thread ficara responsável por enviar as mensagens do cliente
            // mantendo a conexão até o término da execução.                
            
            // Capturando senha do cliente.
            int tentativas = 0; 
            char *senhaTemp;
            while(tentativas < 3){
                            
                // recebendo a senha.
                recebeStr(socketCliente, &senhaTemp);
                tentativas++;
                char ok;
                if(!strncmp(senhaTemp, senha, strlen(senhaTemp))){
                    
                    ok = 'S';
                    write(socketCliente, &ok, 1);
                    free(senhaTemp);
                    break;
                }
                ok = 'N';
                write(socketCliente, &ok, 1);
            }                
            char *nick;
            Link aux;
            while(1){
                        
                if(recebeStr(socketCliente, &nick) <= 0){
                                    
                    close(socketCliente);
                }                       
                aux = pesquisarNick(nick);                    
                char ok;
                if (aux == NULL) { // se login nao existir
                    
                    ok = 'S';
                    write(socketCliente, &ok, 1);
                    break;
                }                   
                ok = 'N';
                write(socketCliente, &ok, 1);
            }
            // Assimilando um file_descripor de socket para o cliente.

            // Adicionando usuário a lista.
            pthread_mutex_lock(&lista);                
            addUserRemoto(nick, novoSocket);        
            pthread_mutex_unlock(&lista);

            free(nick);
        } else { // Se o limite de conexões foi atingido.
            // Informe ao jogador que não é mais possível sua participação.
            
            limiteAtingido (socketCliente);
        }
        qtdeConexoes++;
    }    
    if(socketCliente < 0){
        
        printf("Falha na função accpet.\n");
    }
}

void limiteAtingido (int idSocket){
    
    // enviando mensagem de aviso.
    char *msgLimite = "50:Limite de jogadres atingido. Por favor tente novamente mais tarde";
    enviarStr (idSocket, msgLimite);

    //Free the socket pointer
    close(idSocket);
}

void *escutaCliente(void *socketCliente){
    
    //Id de ientificação do cliente que utiliza a função
    // Podem ser várias threads desta liberadas
    // Logo o idSocket de cada chamada da função escutaCliente 
    // é diferente é um socket diferente.
    
    int idSocket = *(int*)socketCliente, read_size;    
    //int read_size;
    
    // enviando confirmação de pronto para receber nome...
    char *buffer, *donoThread, ac = 'S';                
    write(idSocket, &ac, 1);    
    
    // recebendo o nome do dono da thread.
    if(recebeStr(idSocket, &donoThread) <= 0){
    
        printf("Erro ao receber nome na thread de escuta individual.\n");
        return 0;
    }
    
    // recebe mensagens do cliente.
    Link aux;
    short int qtdeMsg = 0;
    while((read_size = recebeStr(idSocket, &buffer)) > 0){
          
        // extraindo cliente para envio.
        Comando *bloco = extraiMensagem(buffer);        
        // Repassa para broadcast.
        if(!strncmp(bloco->comando, "all", 4)) {
            
            aux = listaLogin->primeiro;
            while (aux != NULL) {

                // não enviar para o mesmo que recebeu.
                if (strcmp(aux->nick, donoThread)) {
                    
                    //TO-DO: Substituir para enviar mesa.
                    //enviarBloco(buffer, donoThread, *aux->socket);
                    qtdeMsg++;
                }
                aux = aux->prox;
            }

            if(!qtdeMsg){
                //TO-DO: Substituir para enviar mesa.
                //enviarBloco("Não há outros usuários no chat.", "SERVIDOR", idSocket);
            }
            qtdeMsg = 0;
            
        }else{ // Enviando mensagem unicast.

            // passando o nick específico para pesquisa.
            aux = pesquisarNick(bloco->comando);
            if (aux != NULL) {
                //TO-DO: Substituir para enviar mesa.
                enviarBloco(buffer, donoThread, *aux->socket);
                
            }else{
                
                //TO-DO: Substituir para enviar mesa.     
                enviarBloco("Este usuário não esta no chat.", "SERVIDOR", idSocket);
            }
        }  
        free(bloco);
    }            
    if(read_size == 0){
                
        pthread_mutex_lock(&lista);
        // Possível local para implantação de mutex.        
        removerUsuario(donoThread);                    
        pthread_mutex_unlock(&lista);
        fflush(stdout);
        
    }else if(read_size == -1){
        
        perror("Falha no recv...\n\n");
    }
         
    //Free the socket pointer
    close(*(int *)socketCliente);
    printf("fechando conexão com cliente...\n");
    return 0;        
}

//TO-DO: Refazer função de fechar conexões.
void fechaConexoes(){
        
    if(!listaVazia()){
        
        Link aux = listaLogin->primeiro;
        while(aux != NULL){

            shutdown(*aux->socket, 2);
            removerUsuario(aux->nick);
            aux = aux->prox;
        }        
    }
    free(listaLogin);
}

void enviarCartas() {

    // Chamando uma função para embaralhar cartas.
    embaralhar (baralho);
    // Aplicando função de distribuir cartas.
    distribuirCartas (jogadores, baralho);
    int numeroJogador, numeroCarta;
    // Enquanto todos os jogadores não estiverem com suas cartas.
    for (numeroJogador = 0; numeroJogador < 4; numeroJogador++){
        
        // Para cada carta sorteada para mão do jogador.
        for (numeroCarta = 0; numeroCarta < 3; numeroCarta++){
            
            // Enviando o nome da carta.
            enviarStr (jogadores[numeroJogador].socket, 
                jogadores[numeroJogador].mao[numeroCarta].nome);
            // Enviando o valor da carta.
            enviarStr (jogadores[numeroJogador].socket, 
                (char *) &jogadores[numeroJogador].mao[numeroCarta].valor);
        }
    }
}

void controleJogo(){

    // Construindo baralho.
    construirBaralho (baralho);
    // armazena o valor da aposta corrente na mesa.
    int valorRodada;

    // Enquanto não houver vencedores.
    while (1){

        // Enviando as cartas para os jogadores.
        enviarCartas ();

        int turnos = 0, vezJogador;
        // Iniciando rodada.
        while(turnos < 3){
            
            // Enviando sinais de permissão para os jogadores.
            while (vezJogador < 4){

            }
        }
    }
}
