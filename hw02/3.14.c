#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
	pid_t pid, pid1;
	pid = fork(); // fork a child process

	if (pid < 0) {
		fprintf(stderr, "Fork Failed!\n");
		return 1;

	} else if (pid == 0) {
		// child process
		pid1 = getpid();
		printf("child: pid\t= %d\n", pid); /* A */
		printf("child: pid1\t= %d\n", pid1); /* B */
	} else {
		// parent process
		pid1 = getpid();
		printf("parent: pid\t= %d\n", pid); /* C */
		printf("parent: pid1\t= %d\n", pid1); /* D */
		wait(NULL);
	}

	return 0;
}
