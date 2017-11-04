all: client.bin ../shared/*

client.bin: main.c
	gcc -o client main.c -I ../shared/ -lssl -lcrypto

clean:
	rm -rf client
