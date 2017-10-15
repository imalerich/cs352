#include "../include/line.h"
#include "../include/args.h"

char * insert_spaces(char * line) {
	if (!line) { return line; }
	// first we need to know how many commands are in line
	size_t num_commands = 1; // each has at least one command
	for (char * tmp = line; *tmp != '\0'; tmp++) {
		if (*tmp == ';' || *tmp == '|') num_commands++; // found an additional command
	}

	size_t buffer_size = strlen(line) + 1 + num_commands;
	char * ret = malloc(sizeof(char) * buffer_size);
	size_t idx = 0;

	for (char * tmp=line; *tmp != '\0'; tmp++) {
		if (*tmp == ';' || *tmp == '|') {
			ret[idx++] = ' ';
			ret[idx++] = *tmp;
		} else {
			ret[idx++] = *tmp;
		}
	}

	while (idx <= buffer_size) { ret[idx++] = ' '; }
	ret[buffer_size] = '\0';

	return ret;
}

char * proc_line(char * line,  History * h) {
	if (!line) { return NULL; }

	// we will need to expand the buffer size as we build the string
	// keep the current buffer size for reference on realloc
	size_t buffer_size = strlen(line) + 1;
	size_t idx = 0;

	// this is the returned string we are going to build up
	// by replacing each !! and !N with their corresponding commands
	// if we cannot find such a command, print an error and return NULL
	char * ret = malloc(sizeof(char) * buffer_size);

	// traverse the string
	for (char * tmp = line; *tmp != '\0'; ++tmp) {
		if (*tmp == '!' && *(tmp+1) == '!') {
			if (!h) { return parse_error("ERROR - No history to search!", line); }

			// get the command, realloc the buffer size, then copy
			// the command over to the new string in the place of '!!'
			char * add = h->command;
			if (!add) { return parse_error("ERROR - No such command exists!", line); }
			idx = add_to_string(&ret, add, idx, &buffer_size);
			
			++tmp; // skip to the next character after !!
		} else if (*tmp == '!' && is_digit(*(tmp+1))) {
			if (!h) { return parse_error("ERROR - No history to search!", line); }

			// build up the history id using following digits
			int cid = 0;
			for (++tmp; is_digit(*tmp); ++tmp) {
				cid = (10 * cid) + get_digit(*tmp);
			}

			char * add = history_get_command(cid, h);
			if (!add) { return parse_error("ERROR - No such command exists!", line); }
			idx = add_to_string(&ret, add, idx, &buffer_size);

			--tmp; // we skipped past the number, back it up a bit
		} else {
			// else nothing special found, simply copy the character
			ret[idx++] = *tmp;
		}
	}

	// remove trailing white spaces, I don't want to store them in the history
	// I only want them entered by insert_spaces
	for (char * tmp = &ret[idx-1]; isspace(*tmp); tmp--) { *tmp = '\0'; }

	ret[idx] = '\0'; // don't forget to NULL terminate our strings
	if (line) { free(line); }
	return ret;
}

void * parse_error(const char * err, char * line) {
	fprintf(stderr, err);
	if (line) { free(line); }
	return NULL;
}

int add_to_string(char ** str, char * add, int idx, size_t * buffer_size) {
	const size_t len = strlen(add);
	// original buffer size - 2 (!!) + length of the string
	*buffer_size = *buffer_size - 2 + len;
	*str = realloc(*str, sizeof(char) * (*buffer_size));

	for (size_t i=0; i<len; ++i) {
		(*str)[idx++] = add[i];
	}

	return idx;
}

bool is_digit(char c) {
	return '0' <= c && c <= '9';
}

int get_digit(char c) {
	return c - '0';
}

char * copy_str(char * string) {
	if (string == NULL) { return NULL; }
	char * copy = malloc(sizeof(char) * (strlen(string) + 1));
	strcpy(copy, string);
	return copy;
}
