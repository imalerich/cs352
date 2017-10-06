#ifndef EXEC_H
#define EXEC_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/history.h"

/**
 * Executes all commands in the given line.
 * Commands are executed in the order in which they appear.
 * Each command may include a '&' character to be run in the background
 * that is, the creating process will not wait for the new process to terminate.
 */
void exec_line(char * line, History * h, int _fd[2], pid_t prev);

/**
 * Executes the line and overlays the current process.
 * If this function returns, an error has occured.
 */
void exec_command(char * command, History * h);

#endif
