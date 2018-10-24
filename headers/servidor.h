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
 *
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 * 
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

// Envia as cartas dos jogadores do servidor para o cliente.
void enviarCartas();

void enviarAnuncioAumentoAposta (int jogadorSolicitante);

void enviarAnuncioAceitaAposta (int jogadorConfirmante);

// Envia as cartas que estão na mesa para os jogadores.
void enviarMesa ();

void enviarValorRodada (int valorRodada);

int jogadorAnterior (int vezJogador);

int proximoJogador (int vezJogador);

// A partir desta função o fluxo do jogo é controlado.
// Os sinais de bloqueio e de permissão para jogar são 
// enviados para os players. Além de toda contabilização
// de pontos e também do exercimento das regras do jogo.
void controleJogo();
#endif