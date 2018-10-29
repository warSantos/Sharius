#ifndef menu_H
#define menu_H

#include "../headers/usuario.h"

// Variável utilizada para contabilizar o numero de conexões,
// estabelecidas no sistema.
int qtdeConexoes;

// Váriável utilizada para criar zonas críticas e sobre
// a variável qtdeConexoes.
pthread_mutex_t qtdeConexoesMutex;

// Variável utilizada para sinalizar para o server o momento de
// iniciar partida.
pthread_mutex_t iniciarPartida;

// Estrutura que comporta os dados sobre todos os jogadores 
// de uma partida (cartas, valor das cartas, nome, id e socket_fd).
Jogador jogadores[4];

// Esta estrutura comportas as catas disponíveis para o jogo, 
// declarada como global para que deste modo evite reconstrução
// do baralho.
Carta baralho[40];

// Armazena as cartas de uma rodada.
Mesa *mesaJogo;

/*
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 */

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
int menuComando(char *buffer);

// Fica aguardando conexões e envio de mesagens.
// Responsável por repassar as mesagens para os demais usuários,
// Receber solicitações de login.
// Gerenciamento de conexões externas.
void escutaSolicitacao();

void *autenticaUsuarios (void *args);

// Recebe as mensagens de um cliente e repassa elas aos demais 
// logados no chat.
void *escutaCliente(void *idSocket);

// Encerra os socket quando o servidor é terminado...
void fechaConexoes();

// Envia as cartas dos jogadores do servidor para o cliente (Sinal 12)..
void enviarCartas();

// Envia uma mensagem para todos os jogadores avisando que
// um jogador solicitou aumento de aposta (pediu, truco, meia,
// nove etc...). A mensagem não é enviada para o jogador que 
// solicitou (Sinal 04).
void enviarAnuncioAumentoAposta (int jogadorSolicitante);

// Envia uma mensagem para todos os jogadores avisando que
// um jogador aceitou aumento de aposta (pediu, truco, meia,
// nove etc...). A mensagem não é enviada para o jogador que 
// aceitou (Sinal 02).
void enviarAnuncioAceitaAposta (int jogadorConfirmante);

// Envia as cartas que estão na mesa para os jogadores (Sinal 11)..
void enviarMesa ();

// Atualiza o valor da rodada dos jogadores após uma alteração
// ocorrer no valor de aposta (alguém pedir um truco por exemplo).
// (Sinal 13).
void enviarValorRodada (int valorRodada);

// Recebe uma mensagem por parâmtro (msg) e envia aos jogadores.
// É utilizada para envio de resultado de turno e rodada (qual
// dupla ganhou ou se ocorreu um empate). (Sinal 14)
void enviarResultado (char *msg);

// Configura variávle nos clientes para impedir que os mesmos
// solicitem pedidos dois pedidos de aposta concecutivos.
void enviarBloqueio (int vezJogador);

// Envia um sinal para todos os clientes. utilizando quando so
// se necessita enviar um sinal e nenhum dado a mais.
void enviarSinal (char *sinal);

// Se um jogador desistir da partida esta função tenta verificar
// através dos bytes lidos verifica posso motivo (interrupção de 
// teclado por exemplo) e fecha conexões com os demais jogadores
// avisando que um jogador saiu da partida.
void jogadorDesistiu (int vezJogador, u_int32_t bytes);

// Retorna o di do jogador "a esquerda" de um jogador do parâmetro
// (vezJogador).
int jogadorAnterior (int vezJogador);

// Retorna o id do jogador "a esquerda" de um jogador do parâmetro
// (vezJogador).
int proximoJogador (int vezJogador);

// A partir desta função o fluxo do jogo é controlado.
// Os sinais de bloqueio e de permissão para jogar são 
// enviados para os players. Além de toda contabilização
// de pontos e também do exercimento das regras do jogo.
void controleJogo();
// envia os numeros do jogadores
void JogadorNumero();
#endif