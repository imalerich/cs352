#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Used a doubly linked list to describe 
 * the users history.
 */
struct _History {
	size_t cid; /**< Command ID of this item in the history. */
	char * command; /**< Pointer to the command returned by getline. */
	struct _History * next; /**< Pointer to the next item in the linked list. */
	struct _History * prev; /**< Pointer to the previous item in the linked list. */
};

typedef struct _History History;

/**
 * Allocates a new history list, containing only the given command.
 */
History * history_alloc(char * command);

/**
 * Removes the given node from the history.
 * This will free h->command as well as h from memory.
 */
void history_remove(History * h);

/**
 * Pushes a new history node in front of 'h' storing the given
 * command as data. A pointer to that node is then returned.
 */
History * history_push(History * head, char * command);

/**
 * Pops the tail (head->prev) of the linked list via
 * the history_remove method.
 */
History * history_pop(History * head);

/**
 * Returns the length of the linked list of 
 * which h is a member. Note, this is always
 * the entire size of the doubly linked list,
 * not just the remaining elements as any
 * element can be treated as the head.
 */
unsigned history_length(History * h);

/**
 * Output the history list to the console.
 * \param h Pointer to history list (most recent is head).
 */
void history_print(History * h);

/**
 * Get the command from the history for the given cid.
 * If no such command is found, NULL is returned.
 */
char * history_get_command(int cid, History * h);

#endif
