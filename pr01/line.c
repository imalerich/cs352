#include "line.h"
#include "args.h"

char * proc_line(char * line,  History * h) {
	// builds a return string by adding each command found in line
	char * ret = get_command(line, h);
	while (line = next_command(line)) {
		// get the next command to be merged  with ret
		char * tmp = get_command(line, h);
		ret = realloc(ret, sizeof(char) * (strlen(ret) + strlen(tmp) + 2));
		ret = strcat(ret, ";");
		ret = strcat(ret, tmp);
		free(tmp);
	}

	free(line);
	return ret;
}

char * get_command(char * line, History * h) {
	if (line == NULL) { return NULL; }
	char * args = split_line(line); // will only contain the first available command
	char * ret = line;

	// there should always be a new line character, remove it
	// will help keep output clean and consistent when doing 
	// multiple commands with history references
	const unsigned length = strlen(line);
	if (line[length-1] == '\n') { line[length-1] = '\0'; }

	if (!strcmp(args, "!!")) {
		if (h == NULL) { ret = NULL; }
		else ret = copy_str(h->command);
	} else if (args[0] == '!') {
		// first character is '!', check if we 
		// can read a number following it
		int cid = atoi(args+1); // args+1 points to the remainder of the string

		ret = copy_str(history_get_command(cid, h));
	}

	free(args);
	return copy_str(ret);
}

char * copy_str(char * string) {
	if (string == NULL) { return NULL; }
	char * copy = malloc(sizeof(char) * (strlen(string) + 1));
	strcpy(copy, string);
	return copy;
}
