Essa proposta consiste em implementar um jogo de truco baseado em turnos. Nesses casos, cada
jogador faz a sua jogada na sua vez enquanto os demais aguardam a sua hora de jogar. Toda
transmissão de dados deverá ser feita entre o cliente e o servidor, nunca entre clientes. O jogo
deverá ser implementado de forma que o servidor do jogo, seja implementado em Unix juntamente
com todas as consistências e regras do mesmo. O cliente deverá ser somente uma interface com
os jogadores. Em função disso, normalmente o programa cliente não faz o redesenho do jogo até
receber uma resposta do servidor, já que este é que verifica desenho das cartas, etc.
As implementações devem ser feitas na linguagem C (C++ pode ser usado para o tratamento
de strings), usando a biblioteca padrão da linguagem.