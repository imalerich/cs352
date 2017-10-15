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
 * @file exec.h
 * @author Ian Malerich
 * @brief Execute processed commands, setup pipes.
 */

/**
 * @brief Executes all commands in the given line.
 *
 * Commands are executed in the order in which they appear.
 * Each command may include a '&' character to be run in the background
 * that is, the creating process will not wait for the new process to terminate.
 */
void exec_line(char * line, History * h, int _fd[2], pid_t prev);

/**
 * @brief Checks whether or not the first word in the given line is 'exit'.
 *
 * If so, returns true, otherwise returns false.
 * Note that the line may have words after or whitespace before.
 * and this function can still return true. However it is a strict
 * requirment that it must be a word, 
 * therefore 'exitcl' for example would return false.
 */
bool should_exit(char * line);

/**
 * @brief Executes the line and overlays the current process.
 *
 * If this function returns, an error has occured.
 */
void exec_command(char * command, History * h);

#endif
