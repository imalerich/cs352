#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "args.h"
#include "history.h"

#define MAX_LINE 80 /* The maximum command length. */
#define HISTORY_LENGTH 10

/**
 * Executes the line and overlays the current process.
 * If this function returns, an error has occured.
 */
void exec_line(char * line, History * h);

/**
 * Returns the command given by line. In most cases,
 * this will just be line. If line is a history command
 * such as '!!' or '!N', then this will return
 * to the corresponding command in history, if no such
 * command exists, NULL is returned instead.
 * Further, if this does NOT return line, then line will
 * be freed from memory.
 */
char * get_command(char * line, History * h);

int main(int argc, char ** argv) {
	char * args[MAX_LINE/2 + 1]; /* Command Line Arguments. */
	int should_run = 1; /* Flag to determine when to exit program. */

	History * h = NULL; // doubly linked list containing history

	while (should_run) {
		printf("osh > ");
		fflush(stdout);

		// read a program the user wishes to run
		char * line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		line = get_command(line, h);

		// run the command
		
		pid_t child = fork();
		if (child < 0) { // fork failed
			fprintf(stderr, "ERROR - Fork Failed - Exiting\n");
			exit(-1);
		} else if (child == 0) { // child process
			exec_line(line, h);
			fprintf(stderr, "ERROR - Exec Failed - Exiting Child Process\n");
			exit(-1);
		} else { // parent process
			// if the user included an '&', 
			// don't wait for the child process to finish
			if (!run_in_background(line)) { wait(NULL); }
		}

		h = history_add_or_create(h, line, HISTORY_LENGTH);
	}

	return 0;
}

char * get_command(char * line, History * h) {
	// skip leading white space when parsing line
	// do NOT modify line as we need to call free on the initial pointer
	char * tmp = line;
	for (; isspace(tmp[0]) && tmp[0] != '\0'; tmp++) { }

	// will at least have a new line at the end, so long
	// as there is some space after "!!", consider it
	// the "!!" command
	if (tmp[0] == '!' && tmp[1] == '!' && isspace(tmp[2])) {
		free(line); // this will overwrite 'line'

		if (h == NULL) { return NULL; }
		else return h->command;
	} else if (tmp[0] == '!') {
		// first character is '!', check if we 
		// can read a number following it
		int cid = atoi(tmp+1); // args+1 points to the remainder of the string
		free(line); // this will overwrite 'line'
		return history_get_command(cid, h);
	}

	return line;
}

void exec_line(char * line, History * h) {
	if (line == NULL) {
		fprintf(stderr, "Error - No Command Found!\n");
		exit(-1);
	}

	char * args = split_line(line);
	char ** arr = get_arg_array(args);

	// args has had whitespace removed, so use that rather than line
	if (!strcmp(args, "history")) {
		history_print(h);
		exit(0);
	}

	execvp(arr[0], arr);
}
