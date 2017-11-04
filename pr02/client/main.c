#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <openssl/sha.h>
#include <encoder.h>

int main(int argc, char ** argv) {
	const int portno = 50001;
	const char * ip_addr = "127.0.0.1";

	/* -- Read the line to send. --- */
	char * line = NULL;
	size_t len  = 0;
	printf("Enter a String to be Sent: ");
	getline(&line, &len, stdin);

	// TODO: Encrypt the line before sending to the server.

	/* --- Calculate the 20-Byte SHA-1 hash. --- */
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(line, len, hash);

	/* --- Setup the socket. --- */
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	/* --- Connect to the server. --- */
	struct hostent * server = gethostbyname(ip_addr);

	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr, server->h_length);

	connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	/* --- Send message to server. --- */
	write(socketfd, line, len);
	write(socketfd, hash, SHA_DIGEST_LENGTH);

	/* --- Output response from server. --- */
	char buffer[256];
	bzero(buffer, 256);
	read(socketfd, buffer, 256);
	printf("%s\n", buffer);

	free(line);
	return 0;
}
