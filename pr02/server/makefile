all: server.bin ../shared/*

server.bin: main.c
	gcc -o server main.c -I ../shared/ -lssl -lcrypto

clean:
	rm -rf server
