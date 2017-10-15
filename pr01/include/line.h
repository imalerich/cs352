#ifndef LINE_H
#define LINE_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "history.h"

/**
 * @file line.h
 * @author Ian Malerich
 * @brief Process a line before execution begins.
 */

/**
 * @brief Adds spaces after ';' in a string.
 *
 * Commands are stored compact in the history.
 * For example "ls;pwd" but my algorithm needs spaces
 * to deterimen the ends of strings, this will insert
 * spaces after the end of each command "ls ;pwd ". 
 * This creates a new string, and does not deallocate the 
 * calling string.
 */
char * insert_spaces(char * line);

/**
 * @brief Replace history tags with actual commands in a line.
 *
 * Uses 'get_command' to process the given line and create a new
 * equivalent line which includes no history commands.
 * For example, on an input of "!!;ls" where !! refers to a pwd command,
 * 	this function will return a new string "pwd;ls".
 * Further, this function will free 'line'. 
 * Thus, it is safe and encouraged to use "line = proc_line(line, h);".
 */
char * proc_line(char * line,  History * h);

/**
 * @brief TRUE if the input character is a digit (0-9), FALSE otherwise.
 */
bool is_digit(char c);

/**
 * @brief Converts the input character into an integer digit.
 *
 * Assumes is_digit(c) == TRUE.
 */
int get_digit(char c);

/**
 * @brief Utility call for proc_line.
 *
 * Copies the input 'add' string to the reference 'str'
 * starting at the location 'add'. This will reallocate str as 
 * necessary, the new buffer size of str will be modified as input.
 * Once done, returns the updated idx, where the caller can continue 
 * writing to 'str'.
 */
int add_to_string(char ** str, char * add, int idx, size_t * buffer_size);

/**
 * @brief Outputs the given error to the console.
 *
 * If line != NULL, line will  be  freed from memory.
 * Always returns NULL;
 */
void * parse_error(const char * err, char * line);

/**
 * @brief Makes a new copy of the input strig in memory.
 *
 * Since the history object is freeing commands
 * as they leave memory, whenever I reference an old
 * commannd, I need to make a copy of the string before
 * adding it to my history object.
 */
char * copy_str(char * string);

#endif
