BIN = osh
OBJ = main.o args.o history.o

all: $(OBJ)
	gcc -o $(BIN) $(OBJ)

main.o: main.c args.h history.h
	gcc -c main.c

args.o: args.h args.c
	gcc -c args.c

history.o: history.h history.c
	gcc -c history.c

clean:
	rm -rf *.o
	rm -rf osh
