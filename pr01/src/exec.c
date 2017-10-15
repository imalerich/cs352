#include <stdbool.h>
#include "../include/exec.h"
#include "../include/args.h"

void exec_line(char * line, History * h, int _fd[2], pid_t prev) {
	// base case, no command to execute
	if (line == NULL) { return; }

	if (should_exit(line)) {
		exit(0);
	}

	// create a pipe to connect between the current process, and the next process
	bool should_pipe = pipe_to_next(line);
	int fd[2];
	if (should_pipe) { 
		// create a pipe, but close both ends of the parent
		pipe(fd); 
	}

	pid_t child = fork();
	if (child < 0) { // fork failed
		exit(-1);
	} else if (child == 0) { // child process
		// this child is on the receiving end of the pipe
		if (_fd) {
			dup2(_fd[0], 0);
			close(_fd[0]);
			close(_fd[1]);

			waitpid(prev, 0, 0);
		}

		// redirect this childs output to the input end of the pipe
		if (should_pipe) {
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
		}

		exec_command(line, h);
		fprintf(stderr, "ERROR - Exec Failed for command '%s' - Exiting Child Process\n", line);
		exit(-1);
	} else { // parent process
		// if the user included an '&', 
		// don't wait for the child process to finish
		// when piping, only wait for the last process
		if (!run_in_background(line)) { 
			waitpid(child, 0, 0); 
			// input process has now sent all it's input to the pipe
			// go ahead and close our input end to the pipe so the
			// next process doesn't try reading from us
			if (should_pipe) { close(fd[1]); }
		}

		// first command done, go ahead and recurse to the next command
		exec_line(next_command(line), h, 
			(should_pipe ? fd : NULL), child);
	}
}

bool should_exit(char * line) {
	char * args = split_line(line);
	if (!strcmp(args, "exit")) {
		if (args) { free(args); args = NULL; }
		return true;
	} else {
		if (args) { free(args); args = NULL; }
		return false;
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
