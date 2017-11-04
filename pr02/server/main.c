#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char ** argv) {
	const int portno = 8675309;

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

	/* --- Connect to a client. --- */
	int clilen = sizeof(client_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);

	/* --- Begin communication. --- */
	char buffer[256];
	bzero(buffer, 256);
	read(newsockfd, buffer, 255); 
	// TODO: buffer is the line
	printf("%s\n", buffer);
	read(newsockfd, buffer, 255);
	// TODO: buffer is the SHA-1 encoding
	printf("%s\n", buffer);
	
	const char * msg = "Message";
	write(newsockfd, msg, strlen(msg));
	close(newsockfd);

	return 0;
}
