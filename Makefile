CC = gcc
CFLAGS = -c -Wall -O3
MOVE = mv *.o obj/
HDRS = src/headers/
SRC = src/source/
FILES = $(wildcard src/source/*)
OBJ = $(FILES:. c=*.o)

sharius: $(OBJ)
	$(CC) $(OBJ) -o sharius

main.o: $(SRC)main.c
	$(CC) $(CFLAGS) $(SRC)main.c
	
usuario.o: $(HDRS)usuario.h $(SRC)usuario.c
	$(CC) $(CFLAGS) $(SRC)usuario.c

strUtils.o:	$(HDRS)strUtils.h	$(SRC)strUtils.c
	$(CC) $(CFLAGS) $(SRC)strUtils.c

run:
	./sharius

clean:
	rm -rf *.o