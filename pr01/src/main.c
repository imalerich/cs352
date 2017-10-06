#include <stdlib.h> 
#include <stdio.h>

#include "../include/args.h"
#include "../include/history.h"
#include "../include/exec.h"
#include "../include/line.h"

#define MAX_LINE 80 /* The maximum command length. */
#define HISTORY_LENGTH 10

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

		// run the command and it to the history
		char * run = insert_spaces(line);
		exec_line(run, h);
		free(run);

		h = history_add_or_create(h, line, HISTORY_LENGTH);
	}

	return 0;
}
