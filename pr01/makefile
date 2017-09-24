BIN = osh
OBJ = main.o args.o history.o line.o exec.o

all: $(OBJ)
	gcc -o $(BIN) $(OBJ)

main.o: src/main.c include/args.h include/history.h include/line.h include/exec.h
	gcc -c src/main.c

args.o: include/args.h src/args.c
	gcc -c src/args.c

history.o: include/history.h src/history.c
	gcc -c src/history.c

line.o: include/line.h src/line.c include/args.h include/history.h
	gcc -c src/line.c

exec.o: include/exec.h src/exec.c include/history.h include/args.h
	gcc -c src/exec.c

clean:
	rm -rf osh
