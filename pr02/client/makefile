OUT = client
SRC = main.c ../shared/encoder.c ../shared/util.c
INC = -I ../shared/ -lssl -lcrypto -lm

all: client.bin ../shared/*

client.bin: main.c
	gcc -std=c99 -o $(OUT) $(SRC) $(INC)

clean:
	rm -rf client
