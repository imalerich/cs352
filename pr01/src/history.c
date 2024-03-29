#include "../include/history.h"

History * history_add_or_create(History * h, char * command, int MAX_HIST) {
	if (command == NULL) { return h; }

	if (h == NULL) {
		h = history_alloc(command);
	} else {
		h = history_push(h, command);
		// trim the history so it only has length HISTORY_LENGTH
		if (h->cid > MAX_HIST) { history_pop(h); }
	}

	return h;
}

void history_remove(History * h) {
	if (h == NULL) { return; }
	// relink the previous and the next nodes
	h->prev->next = h->next;
	h->next->prev = h->prev;
	if (h->command) { free(h->command); h->command = NULL; }
	free(h);
}

unsigned history_length(History * h) {
	if (h == NULL) { return 0; }

	int count = 1;
	for (History * next = h->next; next != h; next = next->next) {
		++count;
	}

	return count;
}

History * history_alloc(char * command) {
	if (command == NULL) { return NULL; }
	History * h = malloc(sizeof(History));
	h->cid = 1;
	h->command = command;
	h->next = h; h->prev = h;
	return h;
}

History * history_push(History * head, char * command) {
	if (head == NULL || command == NULL) { return head; }

	History * push = malloc(sizeof(History));
	push->cid = head->cid + 1;
	push->command = command;

	// link push with head->prev
	push->prev = head->prev;
	push->prev->next = push;

	// link push with head
	push->next = head;
	head->prev = push;

	return push;
}

History * history_pop(History * head) {
	history_remove(head->prev);
	return head;
}

void history_print(History * h) {
	if (h == NULL) { printf("No History...\n"); }
	printf(" %zu %s\n", h->cid, h->command);

	for (History * next = h->next; next != h; next = next->next) {
		printf(" %zu %s\n", next->cid, next->command);
	}
}

char * history_get_command(int cid, History * h) {
	if (h == NULL) { return NULL; }
	if (h->cid == cid) { return h->command; }
	for (History * next = h->next; next != h; next = next->next) {
		if (next->cid == cid) { return next->command; }
	}

	return NULL;
}
