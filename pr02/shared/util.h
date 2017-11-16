#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * The client sends a null-terminator to distinguish
 * the end of the message and the start of the signature.
 * This will read up to the first null terminator, and
 * return a newly allocated buffer containing that string
 * via the 'buffer' parameter.
 * Returns TRUE if EOF is reached, FALSE otherwise.
 */
bool read_buffer(FILE * file, char ** p_buffer);
