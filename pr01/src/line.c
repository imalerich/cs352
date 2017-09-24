#include "../include/line.h"
#include "../include/args.h"

char * proc_line(char * line,  History * h) {
	if (line == NULL) {  return NULL; }

	// builds a return string by adding each command found in line
	char * ret = get_command(line, h);
	char * next = line; // we need to free the whole line, so use a temporary pointer
	while ((next = next_command(next)) && ret != NULL) {
		// get the next command to be merged  with ret
		char * tmp = get_command(next, h);
		if (tmp == NULL) {
			if (line) { free(line); line = NULL; }
			return NULL; 
		}

		ret = realloc(ret, sizeof(char) * (strlen(ret) + strlen(tmp) + 2));
		ret = strcat(ret, ";");
		ret = strcat(ret, tmp);
		if (tmp) { free(tmp); tmp = NULL; }
	}

	if (line) { free(line); line = NULL; }
	return ret;
}

char * get_command(char * line, History * h) {
	if (line == NULL) { return NULL; }
	char * args = split_line(line); // will only contain the first available command
	char * ret = line;

	// there should always be a new line character, remove it
	// will help keep output clean and consistent when doing 
	// multiple commands with history references
	// some commands (ping) like white space at the end though so make it a space
	const unsigned length = strlen(line);
	if (line[length-1] == '\n') { line[length-1] = ' '; }

	if (!strcmp(args, "!!")) {
		// TODO - Parse the string and replace all occurences of !! with
		// the most recent command
		// in  that way ./!! would be valid if the user forgot ./
		
		if (h == NULL) { ret = NULL; }
		else ret = copy_str(h->command);

		if (ret == NULL) { fprintf(stderr, "Error - No History Available!\n"); }
	} else if (args[0] == '!') {
		// TODO - Same thing for !N, only replace occurences of !N
		// don't assume it's the only string

		// first character is '!', check if we 
		// can read a number following it
		int cid = atoi(args+1); // args+1 points to the remainder of the string
		// TODO - Verify that the remainder of the string is in fact a number.

		ret = copy_str(history_get_command(cid, h));

		if (ret == NULL) {fprintf(stderr, "Error - No Command found at: %d\n", cid);  }
	}

	if (args) { free(args); args = NULL; }
	return copy_str(ret);
}

char * copy_str(char * string) {
	if (string == NULL) { return NULL; }
	char * copy = malloc(sizeof(char) * (strlen(string) + 1));
	strcpy(copy, string);
	return copy;
}
