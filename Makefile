CC=gcc
FLAGS=-O3 -Wall -c

OBJ=./obj
HDRS=./headers
SRC=./src

all:	servidor	cliente

servidor:	mservidor	main/main-s.c
	$(CC) main/main-s.c $(OBJ)/strUtils.o $(OBJ)/usuario.o $(OBJ)/servidor.o -o servidor -lpthread

cliente:	mcliente	main/main-c.c
	$(CC) main/main-c.c $(OBJ)/strUtils.o $(OBJ)/usuario.o $(OBJ)/cliente.o -o cliente -lpthread

mservidor:	usuario	$(SRC)/servidor.c	$(HDRS)/servidor.h
	$(CC) $(FLAGS) $(SRC)/servidor.c -o $(OBJ)/servidor.o

mcliente:	usuario	$(SRC)/cliente.c	$(HDRS)/servidor.h
	$(CC) $(FLAGS) $(SRC)/cliente.c -o $(OBJ)/cliente.o

usuario:	truco	$(SRC)/usuario.c	$(HDRS)/usuario.h
	$(CC) $(FLAGS) $(SRC)/usuario.c -o $(OBJ)/usuario.o

truco:	$(SRC)/truco.c	$(HDRS)/truco.h
	$(CC) $(FLAGS) $(SRC)/truco.c

utils:	mk	$(SRC)/strUtils.c	$(HDRS)/strUtils.h
	$(CC) $(FLAGS) $(SRC)/strUtils.c -o $(OBJ)/strUtils.o

mk:	
	mkdir -p $(OBJ) 

clean:
	rm -fr $(OBJ)/* cliente servidor
