#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <encoder.h>

int main(int argc, char ** argv) {
	printf("Enter a String to be Sent: ");

	// read the data package we wish to send
	char * line = NULL;
	size_t len  = 0;
	getline(&line, &len, stdin);

	// calculate the 20-Byte SHA-1 hash
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(line, len, hash);

	free(line);
	return 0;
}
