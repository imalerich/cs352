#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "args.h"
#include "history.h"
#include "line.h"

#define MAX_LINE 80 /* The maximum command length. */
#define HISTORY_LENGTH 10

/**
 * Executes all commands in the given line.
 * Commands are executed in the order in which they appear.
 * Each command may include a '&' character to be run in the background
 * that is, the creating process will not wait for the new process to terminate.
 */
void exec_line(char * line, History * h);

/**
 * Executes the line and overlays the current process.
 * If this function returns, an error has occured.
 */
void exec_command(char * command, History * h);

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
		line = proc_line(line, h);

		// run the command && and it to the history
		exec_line(line, h);
		h = history_add_or_create(h, line, HISTORY_LENGTH);
	}

	return 0;
}

void exec_line(char * line, History * h) {
	// base case, no command to execute
	if (line == NULL) { return; }

	pid_t child = fork();
	if (child < 0) { // fork failed
		fprintf(stderr, "ERROR - Fork Failed - Exiting\n");
		exit(-1);
	} else if (child == 0) { // child process
		exec_command(line, h);
		fprintf(stderr, "ERROR - Exec Failed - Exiting Child Process\n");
		exit(-1);
	} else { // parent process
		// if the user included an '&', 
		// don't wait for the child process to finish
		if (!run_in_background(line)) { wait(NULL); }

		// first command done, go ahead and recurse to the next command
		exec_line(next_command(line), h);
	}
}

void exec_command(char * command, History * h) {
	if (command == NULL) {
		fprintf(stderr, "Error - No Command Found!\n");
		exit(-1);
	}

	char * args = split_line(command);
	char ** arr = get_arg_array(args);

	// args has had whitespace removed, so use that rather than line
	if (!strcmp(args, "history")) {
		history_print(h);
		exit(0);
	}

	execvp(arr[0], arr);
}
