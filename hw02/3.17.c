#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define SIZE 5
int nums[SIZE] = {0, 1, 2, 3, 4};

int main() {
	pid_t pid = fork();

	if (pid == 0) {
		// child process
		for (int i=0; i<SIZE; i++) {
			nums[i] *= -i;
			printf("CHILD: %d ", nums[i]); /* LINE X */
		}
	} else if (pid > 0) {
		// parent process
		wait(NULL);

		for (int i=0; i<SIZE; i++) {
			printf("PARENT: %d ", nums[i]); /* LINE Y */
		}
	}

	return 0;
}
