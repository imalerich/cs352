#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include <openssl/sha.h>
#include <netinet/in.h>
#include <encoder.h>
#include <netdb.h>
#include <util.h>

/**
 * Performs all the necessary handling for a connected client.
 * This should be called on a secondary thread/process
 * so the main thread/process can continue to receive clients.
 */
void * handle_client(void * p_clientfd);

int main(int argc, char ** argv) {
	const int portno = 50001;

	/* --- Setup the socket. --- */
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* --- Setup the server. --- */
	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	listen(sockfd, 5);
	struct sockaddr_in client_addr;

	while (true) {
		/* --- Connect to a client. --- */
		int clilen = sizeof(client_addr);
		int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);

		#ifdef __USE_THREADING 
			/* --- Create a new thread to handle the client. --- */
			pthread_t child;
			int status = pthread_create(&child, NULL, handle_client, (void *)&clientfd);
			if (status) {
				fprintf(stderr, "Error - pthread_create() returned error code: %d\n", status);
				exit(EXIT_FAILURE);
			}

		#else
			/* --- Create a new Process to handle the client. --- */
			pid_t pid = fork();
			if (pid == 0) {
				handle_client((void *)&clientfd);
				exit(0);
			} else if (pid < 0) {
				fprintf(stderr, "Error - call to fork() failed.");
				exit(EXIT_FAILURE);
			}

		#endif
	}

	return 0;
}

/* --- Handle Client. --- */

void * handle_client(void * p_clientfd) {
	int clientfd = *(int *)p_clientfd;
	FILE * file = fdopen(clientfd, "r");

	// the client we are connected to can send multiple commands
	// read every message that they send until we receive an exit
	bool eof_reached = false;
	while (!eof_reached) {
		// first read the actual message sent
		char * message;
		eof_reached = read_buffer(file, &message);

		// if we have reached the end of the file
		// and the client didn't actually give us anything
		// worth looking at, we can go ahead and break out of the loop
		if (eof_reached && strlen(message) == 0) { break; }

		// then read the digital signature
		char * signature;
		eof_reached = read_buffer(file, &signature);

		unsigned char hash[SHA_DIGEST_LENGTH+1];
		SHA1(message, strlen(message), hash);
		hash[SHA_DIGEST_LENGTH] = '\0';
		char * check = stringToEncodedAscii(hash);
		printf("check\n%s\n", check);

		if (strcmp(hash, signature) == 0) {
			const char * msg = "true";
			write(clientfd, msg, strlen(msg)+1);
		} else {
			const char * msg = "false";
			write(clientfd, msg, strlen(msg)+1);
		}

		free(message);
		free(signature);
		free(check);
	}
	
	fclose(file);
	close(clientfd);
}
