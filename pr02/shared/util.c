#include <string.h>
#include "util.h"

bool read_buffer(FILE * file, char ** p_buffer) {
	size_t size = 64;
	char * buffer = malloc(sizeof(char) * size);

	for (int i=0; /* --- */; i++) {
		// need more space, double the buffer size
		if (i == size) {
			size *= 2;
			buffer = realloc(buffer, sizeof(char) * size);
		}

		char c = getc(file);
		if (c == EOF) {
			// end of file reached, terminate and break
			buffer[i] = '\0';
			*p_buffer = buffer;
			return true;
		}

		buffer[i] = c;

		// null terminator found, 
		// already copied, so just break
		if (c == '\0') { break; }
	}

	*p_buffer = buffer;
	return false;
}
