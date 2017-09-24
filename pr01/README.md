# Ian Malerich - CS352 - Project 01

Can be built via executing make in the root directory.
Includes full documentation generated via doxygen in the ./docs/ directory.

## main.c
	Includes the main function and the basic shell loop.
	Maintains the history.

## exec.c
	Provides utilities for executing commands in a line.

## line.c
	Parses utilities for parsing the history out of a line, proc_line is 
	the powerhouse function of this class. Thanks to this method,
	history is stored as literal execution, without references, this even
	includes when trying to run multiple commands.
	So if the user runs "!3;!5" where 3 is "ls;pwd" and 5 is "cat main.c",
	proc_line will create "ls;pwd;cat main.c".

## args.c
	Provides utilities for passing arguments from a line. This pasring
	stops as soon as it finds a ';' character. Thus for a given string,
	you can simply move the pointer of the command you wish to execute
	and use the utilities in this method to get what you need for the
	exec call.

## history.c
	History is implemented via a doubly-linked list. In theory this means
	the memory is dynamic, and you could have it as long as you wanted
	without too much trouble. These files enforce this via a parameter
	which is passed in main.c as the "HISTORY_LENGTH" define statement.
	This value is set to 10 as per assignment specification.
