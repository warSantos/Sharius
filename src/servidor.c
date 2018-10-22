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

void escutaSolicitacao(){
    
    // Cria conexão inicial com clientes e fornece uma thread de
    // recebimento de menagem para o cliente.
    char *senha = malloc(sizeof(char)*16);
    printf("Digite a senha de gerenciamento: ");
    scanf("%15[^\n]s", senha);
    __fpurge(stdin);

    int socketLocal, socketCliente, sizeSockaddr;
    struct sockaddr_in servidor, cliente;
    
    pthread_mutex_init(&qtdeConexoesMutex, NULL);
    pthread_t threadsId[4];
    
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
    Mensagem *args = malloc (sizeof(Mensagem));
    // Encapsulando os argumentos necessarios na função de cadastrar usuario
    // reutilizando a estrutura de enviar mensagem.
    args->msg = senha;
    args->lenght = strlen (senha);

    // Esperando por conexões.
    while(1){

        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        qtdeConexoes++;
        printf ("Inicia aqui as conexões.\n");
        //TO-DO: tratar melhor a possibilidade de erro neste caso.
        if(socketCliente < 0){
            
            break;
        }
        // Se o limite de conexões não foi atingido.
        if (qtdeConexoes < 4){
            // Chama a thread para tratar o cadastro dos clientes.
            args->bytes_read = socketCliente;
            if (pthread_create(&threadsId[qtdeConexoes], NULL,
                autenticaUsuarios, (void *) args)){
                printf ("Erro ao criar thread para cadastrar usuário.\n");
            }
        } else { // Se o limite de conexões foi atingido.
            // Informe ao jogador que não é mais possível sua participação.
            
            limiteAtingido (socketCliente);
        }
    }
    pthread_mutex_destroy (&qtdeConexoesMutex);
    if(socketCliente < 0){
        
        printf("Falha na função accpet.\n");
    }
}

void *autenticaUsuarios (void *args){

    /*
    * Nesta função utiliza-se a estrutura mensagem (senhaESocket) para comportar 
    * parâmetros para chamar passa-los através da função phtread_create.
    * portanto os parâmetros estão configurados da seguinte forma.
    * senhaESocket->bytes_read: socket do cliente.
    * senhaESocket->lenght: tamanho da senha.
    * senhaESocket->msg: a senha do servidor.
    */

    Mensagem *senhaESocket = (Mensagem *) args, *msg;
    
    
    int idJogador, socketCliente = senhaESocket->bytes_read;
    int lenght = senhaESocket->lenght;
    char *senha = senhaESocket->msg;
        
    pthread_mutex_lock (&qtdeConexoesMutex);
    printf ("socketCliente: %d\n", socketCliente);
    printf ("qtdeConexao: %d\n", qtdeConexoes);
    idJogador = qtdeConexoes;
    qtdeConexoes++;
    pthread_mutex_unlock (&qtdeConexoesMutex);

    // Capturando senha do cliente.
    int tentativas = 0;
    while(tentativas < 3){

        // recebendo a senha.
        msg = recebeStr(socketCliente);
        tentativas++;
        char ok;
        if(!strncmp(senha, msg->msg, lenght)){
            
            ok = 'S';
            write(socketCliente, &ok, 1);
            free (msg);
            break;
        }
        ok = 'N';
        write(socketCliente, &ok, 1);
        free (msg);
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
    /*    
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
    
    Link aux;
    // recebe mensagens do cliente.
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
                //enviarBloco(buffer, donoThread, *aux->socket);
                
            }else{
                
                //TO-DO: Substituir para enviar mesa.     
                //enviarBloco("Este usuário não esta no chat.", "SERVIDOR", idSocket);
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
    */
}

//TO-DO: Refazer função de fechar conexões.
void fechaConexoes(){

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
    int vencedorDaRodada,pontosRodada[2] = {0,0};
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
            vencedorDaRodada = vencerRodada(mesa);
            if(vencedorDaRodada == 1 || vencedorDaRodada == 3){
                printf("Dupla 1 ganhou a rodada\n");
            }
            else if(vencedorDaRodada == 2 || vencedorDaRodada == 4){
                printf("Dupla 2 ganhou a rodada\n");
            }
            else{
                printf("Empatou\n");
            }
        }
    }
}
