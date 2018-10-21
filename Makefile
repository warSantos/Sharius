CC=gcc
#FLAGS=-O3 -Wall -c
FLAGS=-c

OBJ=./obj
HDRS=./headers
SRC=./src
MAIN=./main

all:	servidor	cliente

servidor:	main-s
	$(CC) $(OBJ)/strUtils.o $(OBJ)/truco.o $(OBJ)/usuario.o $(OBJ)/servidor.o $(OBJ)/main-s.o -o servidor -lpthread

cliente:	main-c
	$(CC) $(OBJ)/strUtils.o $(OBJ)/truco.o $(OBJ)/usuario.o $(OBJ)/cliente.o $(OBJ)/main-c.o -o cliente -lpthread

main-s:	mservidor	$(MAIN)/main-s.c
	$(CC) $(FLAGS) $(MAIN)/main-s.c -o $(OBJ)/main-s.o

main-c:	mcliente	$(MAIN)/main-c.c
	$(CC) $(FLAGS) $(MAIN)/main-c.c -o $(OBJ)/main-c.o

mservidor:	usuario	$(SRC)/servidor.c	$(HDRS)/servidor.h
	$(CC) $(FLAGS) $(SRC)/servidor.c -o $(OBJ)/servidor.o

mcliente:	usuario	$(SRC)/cliente.c	$(HDRS)/servidor.h
	$(CC) $(FLAGS) $(SRC)/cliente.c -o $(OBJ)/cliente.o

usuario:	truco	$(SRC)/usuario.c	$(HDRS)/usuario.h
	$(CC) $(FLAGS) $(SRC)/usuario.c -o $(OBJ)/usuario.o

truco:	utils	$(SRC)/truco.c	$(HDRS)/truco.h
	$(CC) $(FLAGS) $(SRC)/truco.c -o $(OBJ)/truco.o

utils:	mk	$(SRC)/strUtils.c	$(HDRS)/strUtils.h
	$(CC) $(FLAGS) $(SRC)/strUtils.c -o $(OBJ)/strUtils.o

mk:	
	mkdir -p $(OBJ) 

clean:
	rm -fr $(OBJ)/* cliente servidor
