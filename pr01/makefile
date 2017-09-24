BIN = osh
OBJ = main.o args.o

all: $(OBJ)
	gcc -o $(BIN) $(OBJ)

main.o: main.c args.h
	gcc -c main.c

args.o: args.h args.c
	gcc -c args.c
