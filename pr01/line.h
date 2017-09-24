#ifndef LINE_H
#define LINE_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "history.h"

/**
 * Uses 'get_command' to process the given line and create a new
 * equivalent line which includes no history commands.
 * For example, on an input of "!!;ls" where !! refers to a pwd command,
 * 	this function will return a new string "pwd;ls".
 * Further, this function will free 'line'. 
 * Thus, it is safe and encouraged to use "line = proc_line(line, h);".
 */
char * proc_line(char * line,  History * h);

/**
 * Returns the command given by line. In most cases,
 * this will just be line. If line is a history command
 * such as '!!' or '!N', then this will return
 * to the corresponding command in history, if no such
 * command exists, NULL is returned instead.
 */
char * get_command(char * line, History * h);

/**
 * Makes a new copy of the input strig in memory.
 * Since the history object is freeing commands
 * as they leave memory, whenever I reference an old
 * commannd, I need to make a copy of the string before
 * adding it to my history object.
 */
char * copy_str(char * string);

#endif
