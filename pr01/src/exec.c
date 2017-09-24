#include "../include/exec.h"
#include "../include/args.h"

void exec_line(char * line, History * h) {
	// base case, no command to execute
	if (line == NULL) { return; }

	// TODO - Setup pipes when commands are separated by a '|'

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
