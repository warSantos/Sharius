#ifndef menu_H
#define menu_H

#include "../headers/usuario.h"

// Variável utilizada para contabilizar o numero de conexões,
// estabelecidas no sistema.
int qtdeConexoes;

// Estrutura que comporta os dados sobre todos os jogadores 
// de uma partida (cartas, valor das cartas, nome, id e socket_fd).
Jogador jogadores[4];

// Esta estrutura comportas as catas disponíveis para o jogo, 
// declarada como global para que deste modo evite reconstrução
// do baralho.
Carta baralho[40];

/*
 *
 * Menu de comandos do Servidor.
 * Modulo de gerenciamento de conexões.
 * 
 */

// Função para recember os comandos inseridos pelo usuário no modo "Comando". 
int menuComando(char *buffer);

// Insere os dados dos usuários via socket.
void addUserRemoto(char *nick, int *sock);

// Fica aguardando conexões e envio de mesagens.
// Responsável por repassar as mesagens para os demais usuários,
// Receber solicitações de login.
// Gerenciamento de conexões externas.
void escutaSolicitacao(char *senha);

// Informa a um jogador que sua conexão com o server não pode ser 
// estabelecida devido o limite dos jogadores ter sido atingido.
void limiteAtingido (int idSocket);

// Recebe as mensagens de um cliente e repassa elas aos demais 
// logados no chat.
void *escutaCliente(void *idSocket);

// Encerra os socket quando o servidor é terminado...
void fechaConexoes();

// Envia as cartas dos jogadores do servidor para o cliente.
void enviarCartas();

// A partir desta função o fluxo do jogo é controlado.
// Os sinais de bloqueio e de permissão para jogar são 
// enviados para os players. Além de toda contabilização
// de pontos e também do exercimento das regras do jogo.
void controleJogo();
#endif