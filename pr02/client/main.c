#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <openssl/sha.h>
#include <encoder.h>
#include <util.h>

/**
 * Prompts the user for an ip address.
 * If they give one, it will be stored in 'line'.
 * If they do not given one, a local address will
 * be given by default.
 */
void prompt_ip(char ** addr);

/**
 * Requests a message from the user and store/
 * the resulting string and length into message and len.
 * If the user entered the 'exit' command, this method
 * returns true, false otherwise.
 */
bool read_message(char ** line, size_t * len);

/**
 * Computes the hash for the message, encrypts it,
 * and then sends both the digest and digital signature
 * to the server. An open connection to a server must
 * be open before calling this method.
 */
void encrypt_and_send(int socketfd, char * line, size_t len);

/**
 * After sending a message to the server, this method
 * will wait for a response from the server and output
 * it to the standard output. An open connection
 * to a server must be open before calling this method.
 */
void receive_response(FILE * file);

int main(int argc, char ** argv) {
	const int portno = 50001;
	char * ip_addr;
	prompt_ip(&ip_addr);
	printf("Connecting to: %s\n", ip_addr);
	printf("-----------------------------------------\n");

	/* -- Read the line to send. --- */
	char * line = NULL;
	size_t len  = 0;

	/* --- Setup the socket. --- */
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	/* --- Connect to the server. --- */
	struct hostent * server = gethostbyname(ip_addr);

	if (!server) {
		fprintf(stderr, "ERROR - Failed to connect to server.\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr, server->h_length);

	connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	FILE * file = fdopen(socketfd, "r");

	while (true) {
		if (read_message(&line, &len)) {
			break;
		}

		encrypt_and_send(socketfd, line, len);
		receive_response(file);
	}

	fclose(file);
	close(socketfd);
	free(ip_addr);
	free(line);
	return 0;
}

/* --- Prompt for IP Address. --- */

void prompt_ip(char ** addr) {
	size_t len = 0;
	printf("Enter the ip address you wish to connect to (enter for local): ");
	getline(addr, &len, stdin);

	// the ip address should not include the 
	// carriage return included in getline
	len = strlen(*addr);
	if (isspace((*addr)[len-1])) {
		(*addr)[len-1] = '\0';
		len = len - 1;
	}

	if (len == 0) {
		const char * local = "127.0.0.1";
		*addr = malloc(sizeof(char) * (strlen(local) + 1));
		strcpy(*addr, local);
	}
}

/* --- Read Message. --- */

bool read_message(char ** line, size_t * len) {
	printf("Enter a String to be Sent ('exit' to terminate): ");
	getline(line, len, stdin);

	// don't include the carriage return when we send a message
	*len = strlen(*line);
	if (isspace((*line)[*len-1])) {
		(*line)[*len-1] = '\0';
		*len = *len - 1;
	}

	/* --- Check if the user wants to terminate thei client. */
	static const char * exit_str = "exit";
	return (strcmp(*line, exit_str) == 0);
}

/* --- Encrypt and Send. --- */

void encrypt_and_send(int socketfd, char * line, size_t len) {
	/* --- Send message to server. --- */
	write(socketfd, line, len+1);

	/* --- Calculate the 20-Byte SHA-1 hash. --- */
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(line, len, hash);
	hash[len] = '\0';

	// TODO: Encrypt the hash to send as signature.

	write(socketfd, hash, len+1);
}

/* --- Receive Response. --- */

void receive_response(FILE * file) {
	/* --- Output response from server. --- */
	char * buffer;
	read_buffer(file, &buffer);
	printf("Server responded: %s\n", buffer);
	free(buffer);
}
