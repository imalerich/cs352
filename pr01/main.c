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
			// if the user included an '&', don't wait
			// for the child process to finish
			if (!run_in_background(line)) { wait(NULL); }
		}

		// add the command to the history
		// do this after running the command
		// that way if it is a history command it won't
		// print itself out
		
		if (h == NULL) {
			h = history_alloc(line);
		} else {
			h = history_push(h, line);
			// trim the history so it only has length HISTORY_LENGTH
			if (h->cid > HISTORY_LENGTH) { history_pop(h); }
		}
	}

	return 0;
}

void exec_line(char * line, History * h) {
	char * args = split_line(line);
	char ** arr = get_arg_array(args);

	// args has had whitespace removed, so use that rather than line
	if (!strcmp(args, "history")) {
		history_print(h);
		exit(0);
	} else if (!strcmp(args, "!!")) {
		if (h == NULL) {
			fprintf(stderr, "No Commands in History!\n");
			exit(-3);
		}

		exec_line(h->command, h);
	} else if (args[0] == '!') {
		// first character is '!', check if we 
		// can read a number following it
		int cid = atoi(args+1); // args+1 points to the remainder of the string
		char * cmd = history_get_command(cid, h);
		if (!cmd) {
			fprintf(stderr, "No Such Command in History!\n");
			exit(-2);
		}

		exec_line(cmd, h);
	}

	execvp(arr[0], arr);
}
