#include "LinkedList.h"

LinkedList LinkedList_New() {
	LinkedList new = malloc(sizeof(struct Node));
	new->data = NULL;
	new->next = NULL;
	return new;
}

void LinkedList_Insert(LinkedList list, void* data) {
	LinkedList new = malloc(sizeof(struct Node));
	new->data = data;
	new->next = list->next;
	list->next = new;
}

void LinkedList_Remove(LinkedList list, void* data) {
	LinkedList next = list->next;
	while (next) {
		if (next->data == data) {
			list->next = next->next;
			free(next);
			return;
		}

		list = next;
		next = list->next;
	}
}

// BEWARE! Changes to nodes in 'other' will be reflected in 'list'.
void LinkedList_Append(LinkedList list, LinkedList other) {
	while (list->next)
		list = list->next;

	list->next = other->next;
}

void LinkedList_Clone(LinkedList list, LinkedList other) {
	while (other->next) {
		other = other->next;
		LinkedList_Insert(list, other->data);
	}
}

void LinkedList_Free(LinkedList list) {
	if (list->next)
		LinkedList_Free(list->next);

	free(list);
}