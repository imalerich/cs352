#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "args.h"

char * next_line(char * line) {
	for (char * c = line; *c = '\0'; c++) {
		// found the end of a command, if there is more text
		// following this command, return a pointer to it
		if (*c == ';' && *(c+1) != '\0') { return (c+1); }
	}

	return NULL;
}

char * split_line(char * line) {
	// ignore leading white space characters
	unsigned leading_white = 0;
	for (int i=0; isspace(line[i]) && line[i] != '\0'; i++) {
		++leading_white;
	}

	unsigned length  = 0;
	for (char * c = line; *c != '\0' && *c != ';'; c++) { length++; }

	char * ret = malloc(sizeof(char) * (length + 1 - leading_white));

	for (int i=0; i<length; i++) {
		// process should be executed in the background
		// do not include '&' when creating the arguments array
		if (line[i] == '&') { break; }

		// replace any whitespace character with a NULL terminator
		// can leave any other values as is
		ret[i] = isspace(line[i+leading_white]) ? '\0' : line[i+leading_white];
	}

	ret[length - leading_white] = '\0';
	return ret;
}

char ** get_arg_array(char * args) {
	const unsigned count = num_args(args);
	char ** arr = malloc(sizeof(char*) * (count + 1));

	for (int i=0; i<count; i++) { arr[i] = get_arg(args, i); }
	arr[count] = NULL;
	return arr;
}

bool run_in_background(char * line) {
	if (line == NULL) { return false; }

	for (char * c = line; *c != '\0'; c++) {
		if (*c == '&') { return true; }
		if (*c == ';') { return false; }
	}

	return false;
}

int num_args(char * args) {
	int count = 0;
	for (char * arg = args; arg[0] != '\0'; arg += strlen(arg) + 1) {
		count++;
	}

	return count;
}

char * get_arg(char * args, int i) {
	int idx = 0;
	for (char * arg = args; arg[0] != '\0'; arg += strlen(arg) + 1) {
		if (i == idx) { return arg; }
		idx++;
	}

	return NULL;
}

void print_args(char * line) {
	char * args = split_line(line);
	for (char * arg = args; arg[0] != '\0'; arg += strlen(arg) + 1) {
		printf("%s\n", arg);
	}

	free(args);
}
