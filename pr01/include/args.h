#ifndef ARGS_H
#define ARG_H

#include <stdbool.h>

/**
 * @file args.h
 * @author Ian Malerich
 * @brief Argument parsing from a string input line.
 */

/**
 * @brief Find the next command present in the multi-command line.
 *
 * Given a line containing a command, returns a pointer to the
 * next command stored in the line.
 * If there are no more commands, returns NULL.
 * Commands are separated by ';'.
 * Note that this does not allocate a new string, only returns
 * a pointer into 'line'.
 */
char * next_command(char * line);

/**
 * @brief Split a line by argument by replacing spaces with null terminators.
 *
 * Allocates a new string of length(line)+1-LW.
 * Where LW is the number of leading whitespace characters,
 * these will be ommitted from the generated string.
 * Further, length(line) is the length of line up to the first
 * occurence of the ';' character, which terminates the command.
 * Replaces white space characters in line with NULL
 * terminators for each argument. An extra NULL terminator
 * is then added so the return value may be used as an
 * array of strings.
 * @param line A NULL terminated command to be parsed.
 * @return An array of strings corresponding to the
 * 	arguments of 'line', this will need to be freed
 * 	from memory when it is no longer needed.
 */
char * split_line(char * line);

/**
 * @brief  Get the list of all arguments.
 *
 * Converts the arguments of form char * as returned by split_line
 * into an array of pointers to corresponding locations in that string.
 * This form can then be passed into execvp.
 */
char ** get_arg_array(char * args);

/**
 * @brief Should the input line be run in the background?
 * @return True if a '&' is found before a ';' or the end of the string.
 */
bool run_in_background(char * line);

/**
 * @brief Should the input line pipe to the next command?
 * \return True if the command is delimited by a |, False otherwise.
 */
bool pipe_to_next(char * line);

/**
 * @brief How many argumens are present in the input string?
 *
 * Given arguments which were passed by split_line,
 * returns how many arguments were found in the line.
 * @param Arguments as parsed by split_line.
 * @return Number of arguments found by split_line.
 */
int num_args(char * args);

/**
 * @brief Given arguments as parsed by split_line, and an index
 * where 0<=i<num_args(args), returns a pointer to the first
 * character of the string denoting argumnent i.
 * If no such argument can be found, NULL will be returned.
 */
char * get_arg(char * args, int i);

/**
 * @brief Debug utility: Parses all arguments in the input line.
 * then prints each argument on its own line.
 */
void print_args(char * line);

#endif
