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
    char *senha = "123";
    /*char *senha = malloc(sizeof(char)*16);
    printf("Digite a senha de gerenciamento: ");
    scanf("%15[^\n]s", senha);
    __fpurge(stdin);*/

    int socketLocal, socketCliente, sizeSockaddr, i;
    struct sockaddr_in servidor, cliente;
    
    pthread_mutex_init(&qtdeConexoesMutex, NULL);
    pthread_mutex_init(&iniciarPartida, NULL);
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
    while(qtdeConexoes < QTDE_JOGADORES){

        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, 
            (socklen_t *) &sizeSockaddr);
        printf ("Conexão com cliente: %d iniciada.\n", qtdeConexoes);
        //TO-DO: tratar melhor a possibilidade de erro neste caso.
        if(socketCliente < 0){
            
            close(socketCliente);
        }
        // Chama a thread para tratar o cadastro dos clientes.
        args->bytes_read = socketCliente;
        if (pthread_create(&threadsId[qtdeConexoes], NULL,
            autenticaUsuarios, (void *) args)){
            printf ("Erro ao criar thread para cadastrar usuário.\n");
        }
    }
    // Aguardando as threads de cadastro finalizar.
    for (i = 0; i <= QTDE_JOGADORES; i++){
        pthread_join (threadsId[i], NULL);
    }
    close (socketLocal);
    pthread_mutex_destroy (&qtdeConexoesMutex);    
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
            
            // Enviando signal de confirmação de acesso para o cliente.
            ok = 'S';
            write(socketCliente, &ok, 1);
            jogadores[idJogador].numero = idJogador;
            jogadores[idJogador].socket = socketCliente;
            free (msg);
            return NULL;
        }
        ok = 'N';
        write(socketCliente, &ok, 1);
        free (msg);
    }
    if (tentativas == 3){
        // TO-DO: retornar mensagem de erro para o usuario.
        close (socketCliente);        
    }else if (qtdeConexoes > QTDE_JOGADORES){
        // TO-DO: informar que o usuario demorou para fazer o cadastro
        // e então perdeu sua vaga.
        close (socketCliente);
    }
    // Se a conexão não for aceita.
    pthread_mutex_lock (&qtdeConexoesMutex);
    qtdeConexoes--;
    pthread_mutex_unlock (&qtdeConexoesMutex);
}

void *limiteAtingido (){
    
    // Criando um socket para receber fluxo tcp de dados.
    // AF_INET tipo de conexão sobre IP para redes.
    // SOCK_STREAM protocolo com controle de erros.
    // 0 seleção do protocolo TCP
    int socketLocal, socketCliente, sizeSockaddr;
    struct sockaddr_in servidor, cliente;
    
    socketLocal = socket(AF_INET, SOCK_STREAM, 0);

    if(socketLocal == -1){
        
        printf("Erro ao criar socket local.\n");
        //TO-DO: Tratar retorno desta função de melhor forma.
        return NULL;
    }
        
    servidor.sin_family = AF_INET; // Atribuindo a familia de protocolos para Internet
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(40001); // Setando a porta em que rodara o processo.       
    
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
    
    // Enquanto o jogo não acabar.
    while (1){
        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, 
            (socklen_t *) &sizeSockaddr);
        // enviando mensagem de aviso.
        char *msgLimite = "50:Limite de jogadres atingido. Por favor tente novamente mais tarde";
        enviarStr (socketCliente, msgLimite);

        // Fechando o socket
        close(socketCliente);
    }
    return NULL;
}

//TO-DO: Refazer função de fechar conexões.
void fechaConexoes(){

    int i;
    for (i = 0; i < QTDE_JOGADORES; ++i){
        close(jogadores[i].socket);
    }
}

void enviarCartas() {

    printf ("Enviando cartas.\n");
    // Chamando uma função para embaralhar cartas.
    embaralhar (baralho);
    // Aplicando função de distribuir cartas.
    distribuirCartas (jogadores, baralho);
    int numeroJogador, numeroCarta;
    // Enquanto todos os jogadores não estiverem com suas cartas.
    for (numeroJogador = 0; numeroJogador <= QTDE_JOGADORES; numeroJogador++){
        // Para cada carta sorteada para mão do jogador.
        for (numeroCarta = 0; numeroCarta < 3; numeroCarta++){
            // Enviando o nome da carta.
            enviarStr (jogadores[numeroJogador].socket,
                jogadores[numeroJogador].mao[numeroCarta].nome);
            // Enviando o valor da carta.
            enviarInt (jogadores[numeroJogador].socket, 
                jogadores[numeroJogador].mao[numeroCarta].valor);
        }
    }
}

void controleJogo(){

    // Construindo baralho.
    construirBaralho (baralho);
    // armazena o valor da aposta corrente na mesa.
    // Enviando sinal de partida iniciada (teste).
    int turnos, vezJogador;
    for (vezJogador = 0; vezJogador <= QTDE_JOGADORES; vezJogador++){
        enviarStr (jogadores[vezJogador].socket, "Partida iniciada.\n");
    }
    int valorRodada,resultadoRodada,resultadoTurno;
    int placarTurno[2] = {0,0}; 
    int placarJogo[2] = {0,0};   
    // Enquanto não houver vencedores.
    while (1){
        
        // Enviando as cartas para os jogadores.
        sleep (2);
        enviarCartas ();
        sleep(2);
        valorRodada = 2;    
        // Iniciando rodada.
        for(turnos = 0; turnos < 3; turnos++){
            
            /*// Enviando sinais de permissão para os jogadores.
            while (vezJogador < 4){
                
            }*/
            resultadoRodada = vencerRodada(mesa);
            if(resultadoRodada == 1 || resultadoRodada == 3){
                printf("Dupla 1 ganhou a rodada\n");
                placarTurno[0] = placarTurno[0] + 1; 
            }
            else if(resultadoRodada == 2 || resultadoRodada == 4){
                printf("Dupla 2 ganhou a rodada\n");
                placarTurno[1] = placarTurno[1] + 1;
            }
            else if(resultadoRodada == 5 ){
                printf("Empate : proxima rodada decidira quem ganhará\n");
                placarTurno[0] = placarTurno[0] + 1;
                placarTurno[1] = placarTurno[1] + 1;
            }
        }
        resultadoTurno = vencerTurno(placarTurno);
        if(resultadoTurno == 1){
            printf("Dupla 1 ganhou o Turno\n");
            placarJogo[0] = placarJogo[0] + valorRodada; 

        }
        else if(resultadoTurno == 2){
            printf("Dupla 2 ganhou o Turno\n");
            placarJogo[1] = placarJogo[1] + valorRodada;
        }
        else if(resultadoTurno == 3){
            printf("Empate\n");
        }

        if(placarJogo[0] > 10 || placarJogo[1] > 10){
            break;
        }

        break;
    }
    fechaConexoes();
}
