OUT = server
SRC = main.c ../shared/encoder.c ../shared/util.c
INC = -I ../shared/ -lssl -lcrypto -lm -pthread

## use threading if __use_threading is defined
## otherwise the server will create processes
THREADS = -D'__USE_THREADING'

all: server.bin ../shared/*

server.bin: main.c
	gcc -o $(OUT) $(SRC) $(INC) $(THREADS)

clean:
	rm -rf server
