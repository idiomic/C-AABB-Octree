#include <stdlib.h>

struct Node {
	void* data;
	struct Node* next;
};

typedef struct Node* LinkedList;

LinkedList LinkedList_New();
void LinkedList_Insert(LinkedList, void*);
void LinkedList_Remove(LinkedList, void*);
void LinkedList_Append(LinkedList, LinkedList);
void LinkedList_Clone(LinkedList, LinkedList);
void LinkedList_Free(LinkedList);