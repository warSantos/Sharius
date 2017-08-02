CC = gcc
CFLAGS = -c -Wall -O3
MOVE = mv *.o obj/
HDRS = src/headers/
SRC = src/source/
SRC-C = src/cliente/
SRC-H = src/servidor/
FILES = $(wildcard src/source/*)
OBJ = $(FILES:. c=*.o)

all: sharius-servidor sharius-cliente move

sharius-servidor: $(OBJ)	main-s.o	servidor.o
	$(CC) $(OBJ) main-s.o servidor.o -o sharius-servidor -lpthread

sharius-cliente:	$(OBJ)	main-c.o	cliente.o
	$(CC)	$(OBJ)	main-c.o cliente.o -o sharius-cliente -lpthread

main-s.o:	$(SRC-H)main-s.c
	$(CC) $(CFLAGS) $(SRC-H)main-s.c

main-c.o: $(SRC-C)main-c.c
	$(CC) $(CFLAGS) $(SRC-C)main-c.c

servidor.o:	$(SRC-H)servidor.h	$(SRC-H)servidor.c
	$(CC) $(CFLAGS) $(SRC-H)servidor.c

cliente.o:	$(SRC-C)cliente.h	$(SRC-C)cliente.c
	$(CC) $(CFLAGS) $(SRC-C)cliente.c

usuario.o: $(HDRS)usuario.h $(SRC)usuario.c
	$(CC) $(CFLAGS) $(SRC)usuario.c

strUtils.o:	$(HDRS)strUtils.h	$(SRC)strUtils.c
	$(CC) $(CFLAGS) $(SRC)strUtils.c

run-cliente:
	./sharius-cliente

run-servidor:
	./sharius-servidor

clean:
	rm -rf obj/*.o
move:
	$(MOVE)
