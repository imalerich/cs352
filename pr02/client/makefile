OUT = client
SRC = main.c ../shared/encoder.c ../shared/util.c
INC = -I ../shared/ -lssl -lcrypto -lm

all: client.bin ../shared/*

client.bin: main.c
	gcc -o $(OUT) $(SRC) $(INC)

clean:
	rm -rf client
