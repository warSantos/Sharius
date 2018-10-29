#include "../headers/servidor.h"

void escutaSolicitacao(){
    
    // Cria conexão inicial com clientes e fornece uma thread de
    // recebimento de menagem para o cliente.
    char *senha = malloc(sizeof(char)*16);
    printf("Digite a senha de gerenciamento: ");
    scanf("%15[^\n]s", senha);
    __fpurge(stdin);

    int socketLocal, socketCliente, sizeSockaddr, i;
    struct sockaddr_in servidor, cliente;
    
    pthread_mutex_init(&qtdeConexoesMutex, NULL);
    pthread_mutex_init(&iniciarPartida, NULL);
    pthread_t threadsId[QTDE_JOGADORES];
    
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
        close(socketLocal);       
        exit(1);
    }
    
    // Limitando o número de conexões que o servidor vai aceitar para 4 conexões.
    listen(socketLocal, QTDE_JOGADORES);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    Mensagem *args = malloc (sizeof(Mensagem));
    // Encapsulando os argumentos necessarios na função de cadastrar usuario
    // reutilizando a estrutura de enviar mensagem.
    args->msg = senha;
    args->lenght = strlen (senha);

    // Esperando por conexões.
    while(qtdeConexoes < QTDE_JOGADORES - 1){

        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, 
            (socklen_t *) &sizeSockaddr);        
        if(socketCliente < 0){
            close(socketCliente);
        }else {
            printf ("Conexão com cliente: %d iniciada.\n", qtdeConexoes);
        }
        // Chama a thread para tratar o cadastro dos clientes.
        args->bytes_read = socketCliente;
        if (pthread_create(&threadsId[qtdeConexoes], NULL,
            autenticaUsuarios, (void *) args)){
            printf ("Erro: falha ao criar thread para cadastrar usuário.\n");
        }
    }
    // Aguardando as threads de cadastro finalizar.
    for (i = 0; i < QTDE_JOGADORES; i++){
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
        close (socketCliente);        
    }else if (qtdeConexoes > QTDE_JOGADORES){
        close (socketCliente);
    }
    // Se a conexão não for aceita.
    pthread_mutex_lock (&qtdeConexoesMutex);
    qtdeConexoes--;
    pthread_mutex_unlock (&qtdeConexoesMutex);
}

void fechaConexoes(){

    int i;
    for (i = 0; i < QTDE_JOGADORES; ++i){
        // Envia sinal de desistência de jogador.
        enviarStr (jogadores[i].socket, "31");
        sleep (0.5);
        // Fecha socket da conexão com o jogador.
        close(jogadores[i].socket);
    }
}

void controleJogo(){

    // Construindo baralho.
    construirBaralho (baralho);
    int turnos, vezJogador, jogadas, maoDe10, trucoMao10;
    u_int32_t valorRodada, resultadoRodada, resultadoTurno;
    u_int32_t recusoAumento, primeiroTurno, jogadorRodada = 0;
    int tentos[5] = {2, 4, 8, 10, 12};
    int placarTurno[2];
    int placarJogo[2] = {0,0};
    int respostaAumento, jogadoresAumento[2] = {0,0}, jogadorSolicitante, BloquearTruco[2] = {-1,-1};
    Mensagem *msg, *mao10[JOGADORES_POR_DUPLA];
    
    // Enviando mensagem inicial para jogadores (teste da conexão).
    for (vezJogador = 0; vezJogador < QTDE_JOGADORES; vezJogador++){
        enviarStr (jogadores[vezJogador].socket, "Partida iniciada.\n");
    }

    // Enquanto não houver vencedores.
    while (1){
        sleep (1);
        // Enviando cartas para os jogadores.
        JogadorNumero();
        enviarCartas ();
        sleep(1);
        valorRodada = 0;
        maoDe10 = 0;
        vezJogador = jogadorRodada;
        // Enviando sinal de desbloquio de aposta (bloqueioAumento).
        enviarSinal("17");
        if (placarJogo[0] == 10 && placarJogo[1] != 10){
            // Enviando sinal pergunta se dupla vai aceitar ir na mão de 10.
            int j, cont = 0;
            for (j = 0; j < QTDE_JOGADORES; j += 2){
                enviarStr (jogadores[j].socket, "15");
                cont++;
            }
            // Recebendo resposta dos jogadores.
            cont = 0;
            for (j = 0; j < QTDE_JOGADORES; j += 2){
                mao10[cont] = recebeStr (jogadores[j].socket);
                cont++;
            }
            // Caso algum jogador não responda o servidor.
            cont = 0;
            for (j = 0; j < QTDE_JOGADORES; j += 2){
                jogadorDesistiu (j, mao10[cont]->bytes_read);
                cont++;
            }
            // Se algum dos dois jogadores aceitarem.
            for (j = 0; j < JOGADORES_POR_DUPLA; j++){
                if (!strncmp (mao10[j]->msg, "02", 3)){
                    maoDe10 = 1;
                }
            }
            if (maoDe10 != 1) { // Se os dois não aceitarem.
                placarJogo[1] += 2;
                maoDe10 = -1;
            }else{ // Se algum aceitou.
                valorRodada++;
                // Enviando sinal de ajuste de variável (valorMao10).
                enviarSinal ("18");
            }
            for (j = 0; j < JOGADORES_POR_DUPLA; j++){
                free (mao10[j]);
            }

        }else if(placarJogo[1] == 10 && placarJogo[0] != 10){
            // Enviando sinal pergunta se dupla vai aceitar ir na mão de 10.
            int j, cont = 0;
            for (j = 1; j < QTDE_JOGADORES; j += 2){
                enviarStr (jogadores[j].socket, "15");
                cont++;
            }
            // Recebendo resposta dos jogadores.
            cont = 0;
            for (j = 1; j < QTDE_JOGADORES; j += 2){
                mao10[cont] = recebeStr (jogadores[j].socket);
                cont++;
            }
            // Caso algum jogador não responda o servidor.
            cont = 0;
            for (j = 1; j < QTDE_JOGADORES; j += 2){
                jogadorDesistiu (j, mao10[cont]->bytes_read);
                cont++;
            }
            // Se algum dos dois jogadores aceitarem.
            for (j = 0; j < JOGADORES_POR_DUPLA; j++){
                if (!strncmp (mao10[j]->msg, "02", 3)){
                    maoDe10 = 1;
                }
            }
            if (maoDe10 != 1) { // Se os dois não aceitarem.
                placarJogo[1] += 2;
                maoDe10 = -1;
            }else{ // Se algum aceitou.
                valorRodada++;
                // Enviando sinal de ajuste de variável (valorMao10).
                enviarSinal ("18");
            }
            for (j = 0; j < JOGADORES_POR_DUPLA; j++){
                free (mao10[j]);
            }
        }
        // Se os jogadores estiverem e aceitarem mão de 10.
        if (maoDe10 != -1){
            // Iniciando rodada.
            trucoMao10 = 0;
            respostaAumento = 0;
            recusoAumento = 0;
            placarTurno[0] = 0;
            placarTurno[1] = 0;
            // Atualizando o valor da rodada nos jogadores para 2.
            enviarValorRodada (tentos[valorRodada]);
            // Iniciando rodada.
            for(turnos = 0; turnos < 3; turnos++){
                mesaJogo = calloc (1, sizeof(Mesa));
                printf ("Turno: %d.\n", turnos);
                // Enquanto cada jogador não realizar sua jogada.
                for (jogadas = 0; jogadas < QTDE_JOGADORES; jogadas++){
                    printf ("Atual valor da rodada: %d.\n", tentos[valorRodada]);
                    printf ("Vez do Jogador: %d.\n", vezJogador);
                    // Se existir cartas na mesa envie.
                    if (mesaJogo->tamMesa > 0){
                        printf ("Enviando cartas na mesa.\n");
                        enviarMesa();
                    }
                    // Envia sinal de permissao para o jogador.
                    enviarStr (jogadores[vezJogador].socket, "10");
                    // Recebe resposta do jogador da vez.
                    msg = recebeStr (jogadores[vezJogador].socket);
                    jogadorDesistiu (vezJogador, msg->bytes_read);
                    // Se for a solicitacao de jogar uma carta.
                    if (!strncmp (msg->msg, "00", msg->lenght)){
                        // Recebendo o nome da carta.
                        msg = recebeStr (jogadores[vezJogador].socket);
                        jogadorDesistiu (vezJogador, msg->bytes_read);
                        // Adicionando carta a mesa.
                        strncpy (mesaJogo->cartas[mesaJogo->tamMesa].nome, msg->msg, 3);
                        // Recebendo o valor da carta.
                        mesaJogo->cartas[mesaJogo->tamMesa].valor = recebeInt (jogadores[vezJogador].socket);
                        // Registrando qual jogador jogou qual carta.            
                        mesaJogo->numeroJogador[mesaJogo->tamMesa] = vezJogador;
                        // Aumentando a quantidade de cartas na mesa.
                        mesaJogo->tamMesa++;
                    // Se for solicitação de aumento de aposta.
                    }else if (!strncmp (msg->msg, "01", msg->lenght)){
                        // Se for uma resposta a uma solicitação de aumento anterior.
                        if (respostaAumento){
                            // Então aumente o valor da rodada para o valor da solicitação anterior.
                            valorRodada++;
                            // Atualize o valor de aposta dos jogadores.
                            enviarValorRodada (tentos[valorRodada]);
                            // Enviar a solicitação para todos jogadores verem a solicitação
                            // da mensagem e qual jogador pediu truco.
                            enviarAnuncioAumentoAposta (vezJogador);

                            // Se o jogador que quiser aumentar a aposta for o jogador intimado inicialmente.
                            if (jogadorSolicitante){
                                enviarStr (jogadores[jogadoresAumento[0]].socket, "01");
                                vezJogador = jogadorAnterior (vezJogador);
                                vezJogador = jogadorAnterior (vezJogador);
                                jogadorSolicitante = 0;
                            }else{ // Se for o jogador que intimou inicialmente.
                                enviarStr (jogadores[jogadoresAumento[1]].socket, "01");
                                jogadorSolicitante = 1;
                            }
                        }else {
                            // Se for mão de 10 e o jogador pediu truco.
                            if (maoDe10 == 1){
                                // Dupla do jogador que pediu truco vezJogador % 2.
                                if (vezJogador % 2){
                                    // Force a vitória da dupla (0 - 2).
                                    placarTurno[0] = 2;
                                    placarTurno[1] = 0;
                                }else {
                                    // Force a vitória da dupla (1 - 3).
                                    placarTurno[1] = 2;
                                    placarTurno[0] = 0;
                                }
                                trucoMao10 = 1;
                                break;
                            } // Se não for mão de 10.
                            else {
                                respostaAumento = 1;
                                // Armazenando qual jogador iniciou aumento de aposta.
                                jogadoresAumento[0] = vezJogador;
                                // Enviar a solicitação para todos jogadores verem a solicitação
                                // da mensagem e qual jogador pediu truco.
                                enviarAnuncioAumentoAposta (vezJogador);
                                // Enviando solicitação de truco para o jogador a direita.
                                int jogadorDireita = proximoJogador (vezJogador);
                                // Armazenando qual jogador respondera ao aumento incial.
                                jogadoresAumento[1] = jogadorDireita;
                                // Enviando sinal de aumento de aposta para ele.
                                enviarStr (jogadores[jogadorDireita].socket, "01");
                                // Configurando flag jogadorSolicitante.
                                jogadorSolicitante = 1;
                            }
                        }
                        // Enviando sinal de bloqueio de aumento de aposta.
                        enviarBloqueio (vezJogador);
                        jogadas--;
                    }// Se for uma aceitação de aumento de aposta.
                    else if (!strncmp (msg->msg, "02", msg->lenght)){
                        // Enviar sinal de aumento aceito para todos menos para quem confirmou.
                        enviarAnuncioAceitaAposta (vezJogador);
                        valorRodada++;
                        // Atualize o valor de aposta dos jogadores.
                        enviarValorRodada (tentos[valorRodada]);
                        // Se o jogador que respondeu este aumento foi o jogador intimado inicialmente.
                        if (vezJogador == jogadoresAumento[1]){
                            // Retorna ao jogador a esquerda do jogador que solicitou incialmente.
                            vezJogador = jogadorAnterior (vezJogador);   
                        }
                        vezJogador = jogadorAnterior (vezJogador);
                        respostaAumento = 0;
                        jogadas--;
                    }// Se for um recuso de aumento de aposta.
                    else if (!strncmp (msg->msg, "03", msg->lenght)){
                        // Se for a dupla (0 - 2) que desistiu.
                        if (vezJogador == 0 || vezJogador == 2){
                            placarJogo[1] += tentos[valorRodada];
                        }else { // se for a dupla (1 - 3) que desistiu.
                            placarJogo[0] += tentos[valorRodada];
                        }
                        recusoAumento = 1;
                        break;
                    }
                    vezJogador = proximoJogador (vezJogador);
                    free (msg);
                } // Todos os jogadores fizeram suas jogadas.
                // Se a rodada chegou ao fim sem ser por um recuso de aumento de aposta.
                if (!recusoAumento){
                    // Se algum jogador pediu truco na mao de 10 não analise as cartas da mesa.
                    char resultado[75];
                    if (!trucoMao10){
                        // Identifica qual dupla venceu a rodada.
                        resultadoRodada = vencerTurno(mesaJogo);
                        if(turnos == 0){
                            primeiroTurno = resultadoRodada;
                        }
                        if(resultadoRodada == 0 || resultadoRodada == 2){
                            // Enviando anúncio da dupla que venceu (0 - 2).
                            sprintf (resultado, "\nDupla vencedora do turno: (0 - 2).\n");
                            enviarResultado (resultado);
                            // Aumentando os pontos da dupla vencedora.
                            placarTurno[0]++;
                            // Definindo qual jogador vai inciar a próxima jogada.
                            vezJogador = resultadoRodada;
                        }
                        else if(resultadoRodada == 1 || resultadoRodada == 3){
                            // Enviando anúncio da dupla que venceu (1 - 3).
                            sprintf (resultado, "\nDupla vencedora do turno: (1 - 3).\n");
                            enviarResultado (resultado);
                            // Aumentando os pontos da dupla vencedora.
                            placarTurno[1]++;
                            // Definindo qual jogador vai inciar a próxima jogada.
                            vezJogador = resultadoRodada;
                        }
                        // Se for um empate (valor 5).
                        else {
                            sprintf (resultado, "Empate.\n");
                            enviarResultado (resultado);
                            placarTurno[0]++;
                            placarTurno[1]++;
                        }
                    }
                    free (mesaJogo);  


                    sprintf(resultado ,"\n \t\t\t __________________\n \t\t\t|                  |\n \t\t\t|   Placar Turno   |\n \t\t\t|\033[34m Dupla (0 - 2): %d \033[37m|\n"
                        " \t\t\t|\033[31m Dupla (1 - 3): %d \033[37m|\n \t\t\t|__________________|\n ", placarTurno[0], placarTurno[1]);
                    enviarResultado (resultado);
                    resultadoTurno = vencerRodada (placarTurno,primeiroTurno,turnos);
                    if(resultadoTurno == 1){
                        placarJogo[0] = placarJogo[0] + tentos[valorRodada];
                        break;
                    }
                    else if(resultadoTurno == 2){                    
                        placarJogo[1] = placarJogo[1] + tentos[valorRodada];
                        break;
                    }
                    else if(resultadoTurno == 3){
                        break;
                    }
                }else { 
                    // Se houve um recuso de aumento de aposta encerre a rodada.
                    break;
                }
            }
        }
        char resultadoJogo[75];
        sprintf(resultadoJogo,"\n-------------------------------------------------------------------------------\n");
        enviarResultado(resultadoJogo);   
        sprintf(resultadoJogo ,"\n \t\t\t ____________________\n \t\t\t|                    |\n \t\t\t|  Pontuacao Geral   |\n \t\t\t|\033[34m  Dupla (0 - 2): %d \033[37m |\n"
                          " \t\t\t|\033[31m  Dupla (1 - 3): %d \033[37m |\n \t\t\t|____________________|\n ", placarJogo[0], placarJogo[1]);
        enviarResultado (resultadoJogo);
        jogadorRodada = proximoJogador(jogadorRodada);
        if(placarJogo[0] > 10 || placarJogo[1] > 10){
            break;
        }
    }
    fechaConexoes();
}

int proximoJogador (int vezJogador){

    // Se estiver no último jogador.
    if (vezJogador == QTDE_JOGADORES - 1){
        return 0;
    }
    vezJogador++;
    return vezJogador;
}

int jogadorAnterior (int vezJogador){
    // Se estiver no primeiro jogador.
    if (vezJogador == 0){
        return (QTDE_JOGADORES - 1);
    }
    vezJogador--;
    return vezJogador;
}

void enviarCartas() {

    printf ("Enviando cartas.\n");
    // Chamando uma função para embaralhar cartas.
    embaralhar (baralho);
    // Aplicando função de distribuir cartas.
    distribuirCartas (jogadores, baralho);
    int numeroJogador, numeroCarta;
    // Enquanto todos os jogadores não estiverem com suas cartas.
    for (numeroJogador = 0; numeroJogador < QTDE_JOGADORES; numeroJogador++){
        // Envie o sinal de envio das cartas para os jogadores.
        enviarStr (jogadores[numeroJogador].socket, "12");
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

void enviarMesa (){
    
    int carta, jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // Enviando código de envio de mesa.
        enviarStr (jogadores[jogador].socket, "11");
        // Enviando o tamanho da mesa.
        enviarInt (jogadores[jogador].socket, mesaJogo->tamMesa);
        for (carta = 0; carta < mesaJogo->tamMesa; carta++){
            // Enviando o nome da carta para os jogadores.
            enviarStr (jogadores[jogador].socket, mesaJogo->cartas[carta].nome);
        }
    }
}

void enviarAnuncioAumentoAposta (int jogadorSolicitante){

    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // Não envie para o jogador que pediu o truco.
        if (jogador != jogadorSolicitante){
            // Envie sinal de anúncio de aumento de aposta.
            enviarStr (jogadores[jogador].socket, "04");
            // Envie o numero do jogador que pediu o anuncio.
            enviarInt (jogadores[jogador].socket, jogadorSolicitante);
        }
    }
}

void enviarAnuncioAceitaAposta (int jogadorConfirmante){

    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // Não envie para o jogador que pediu o truco.
        //if (jogador != jogadorConfirmante){
            // Envie sinal de anúncio de aumento de aposta.
            enviarStr (jogadores[jogador].socket, "02");
            // Envie o numero do jogador que aceitou aumento.
            enviarInt (jogadores[jogador].socket, jogadorConfirmante);
        //}
    }
}

void enviarValorRodada (int valorRodada){

    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // Enviando o código de envio de atualização valor da rodada.
        enviarStr (jogadores[jogador].socket, "13");
        // Enviando o valor da rodada.
        enviarInt (jogadores[jogador].socket, valorRodada);
    }
}

void enviarResultado (char *msg){

    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // Não envie para o jogador que pediu o truco.        
        // Enviando sinal de informação de resultado.
        enviarStr (jogadores[jogador].socket, "14");
        // Enviando mensagem com resultado.
        enviarStr (jogadores[jogador].socket, msg);
    }
}

void enviarBloqueio (int vezJogador){
    // Enviando sinal de bloqueio de aumento aposta.
    int jogador, bloqueioAposta = 1;
    if (vezJogador % 2 == 0){
        jogador = 0;
    }else {
        jogador = 1;
    }
    for (; jogador < QTDE_JOGADORES; jogador += 2){
        // envia sinal de bloqueio de aumento de aposta para dupla.
        enviarStr (jogadores[jogador].socket, "16");
    }
}

void enviarSinal (char *sinal){
    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
        // envia sinal de desbloqueio de aumento de aposta para dupla.
        enviarStr (jogadores[jogador].socket, sinal);
    }
}

void jogadorDesistiu (int vezJogador, u_int32_t bytes){

    if (bytes <= 0){
        int jogador;
        for (jogador = 0; jogador < QTDE_JOGADORES; jogador++){
            if (jogador != vezJogador){
                // Enviando sinal de que jogador desistiu.
                enviarStr (jogadores[jogador].socket, "30");
                // Enviando número do joagdor que desistiu.
                enviarInt (jogadores[jogador].socket, vezJogador);
                // Fechando conexão com servidor.
                close (jogadores[jogador].socket);
            }
        }
        exit (1);
    }
}
void JogadorNumero(){
    char numeroJogador[75];
    int jogador;
    for (jogador = 0; jogador < QTDE_JOGADORES; ++jogador){
        enviarStr (jogadores[jogador].socket,"14");
        sprintf(numeroJogador,"Jogador %i \n",jogador);
        enviarStr(jogadores[jogador].socket,numeroJogador);
    }
}