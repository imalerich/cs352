#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "args.h"

#define MAX_LINE 80 /* The maximum command length. */

int main(int argc, char ** argv) {
	char * args[MAX_LINE/2 + 1]; /* Command Line Arguments. */
	int should_run = 1; /* Flag to determine when to exit program. */

	while (should_run) {
		printf("osh > ");
		fflush(stdout);

		// read a program the user wishes to run
		char * line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);

		/**
		 * After reading the users input:
		 * (1) fork a child process - DONE
		 * (2) invoke child process via execvp - DONE
		 * (3) if command included &, parent will invoke wait
		 */

		pid_t child = fork();
		if (child < 0) { // fork failed
			fprintf(stderr, "ERROR - Fork Failed - Exiting\n");
			exit(-1);
		} else if (child == 0) { // child process
			char * args = split_line(line);
			char ** arr = get_arg_array(args);

			execvp(arr[0], arr);
			fprintf(stderr, "ERROR - Exec Failed - Exiting Child Process\n");
			exit(-1);
		} else { // parent process
			// if the user included an '&', don't wait
			// for the child process to finish
			if (!run_in_background(line)) { wait(NULL); }
		}
	}

	return 0;
}
