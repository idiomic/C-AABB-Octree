#include "Octree.h"

Octree Octree_New(vec3 pos, int size) {
	Octree octree = malloc(sizeof(struct Octree_Cell));

	// Lets store these values here so they are safe
	octree->size = size;
	octree->x = pos[0];
	octree->y = pos[1];
	octree->z = pos[2];

	// I support encapsulation
	octree->values = LinkedList_New();

	// This code assumes uncreated children are NULL
	for (int i = 0; i < 8; i++)
		octree->children[i] = NULL;

	return octree;
}

void Octree_NewChild(Octree parent, int index) {
	Octree child = malloc(sizeof(struct Octree_Cell));

	// child size is one power of two smaller than its parent
	child->size = parent->size - 1;

	// child position is offset from its parents by its size
	int offset = 1 << child->size;
	child->x = parent->x + (index & 1<<0 ? -offset : offset);
	child->y = parent->y + (index & 1<<1 ? -offset : offset);
	child->z = parent->z + (index & 1<<2 ? -offset : offset);

	// I support encapsulation
	child->values = LinkedList_New();

	// This code assumes uncreated children are NULL
	for (int i = 0; i < 8; i++)
		child->children[i] = NULL;

	parent->children[index] = child;
}

Octree Octree_GetChild(Octree octree, vec3 pA, vec3 pB, int no_create) {
	// The minimum point's position relative to the octree
	int x = pA[0] < octree->x;
	int y = pA[1] < octree->y;
	int z = pA[2] < octree->z;

	// If the maximum point is not in the same child, return the parent
	int index;
	if (x == (pB[0] < octree->x) &&
		y == (pB[1] < octree->y) &&
		z == (pB[2] < octree->z))
		index = (x<<0) + (y<<1) + (z<<2);
	else
		return NULL;

	// If the child exists, return it
	if (octree->children[index])
		return octree->children[index];

	// Else if it DNE because it is too fine, return the parent
	else if (no_create || 1 << octree->size - 1 < MINIMUM_OCTREE_CELL_SIZE)
		return NULL;

	// Else create the child
	Octree_NewChild(octree, index);
	return octree->children[index];
}

Octree Octree_GetDescendent(Octree octree, vec3 pA, vec3 pB, int no_create){
	Octree last;
	Octree cur = octree;

	do {
		last = cur;
		cur = Octree_GetChild(cur, pA, pB, no_create);
	} while (cur);

	return last;
}

void Octree_Insert(Octree octree, vec3 pA, vec3 pB, void* data){
	return LinkedList_Insert(
		Octree_GetDescendent(octree, pA, pB, CREATE)->values,
		data);
}

void Octree_Remove(Octree octree, vec3 pA, vec3 pB, void* data) {
	return LinkedList_Remove(
		Octree_GetDescendent(octree, pA, pB, NO_CREATE)->values,
		data);
}

void Octree_Append(Octree octree, LinkedList result) {
	LinkedList_Append(result, octree->values);

	for (int i = 0; i < 8; i++)
		if (octree->children[i])
			Octree_Append(octree->children[i], result);
}

LinkedList Octree_Intersection(Octree octree, vec3 pA, vec3 pB) {
	LinkedList result = LinkedList_New();

	Octree last;
	Octree cur = octree;

	// Find the cell containing pA and pB, appending ancester
	// values along the way
	do {
		last = cur;
		LinkedList_Append(result, last->values);
		cur = Octree_GetChild(cur, pA, pB, NO_CREATE);
	} while (cur);

	// Every descendent's values are now in the desired area
	for (int i = 0; i < 8; i++)
		if (last->children[i])
			Octree_Append(last->children[i], result);

	return result;
}

void Octree_Free(Octree octree) {
	for (int i = 0; i < 8; i++)
		if (octree->children[i])
			Octree_Free(octree->children[i]);

	free(octree);
}

void Octree_Print(Octree octree, int level) {
	char* l = malloc(sizeof(char) * 2 * level + 1);
	for (int i = 0; i < level; i++) {
		l[2*i] = '|';
		l[2*i + 1] = '-';
	}
	l[2 * level] = '\0';

	printf("%sCenter: <%f, %f, %f>\n", l, octree->x, octree->y, octree->z);

	printf("%sValues: {", l);
	LinkedList ll = octree->values;
	while (ll->next) {
		ll = ll->next;
		printf(" %d, ", *((int*) ll->data));
	}
	printf(" }\n");

	printf("%sChildren: {\n", l);
	for (int i = 0; i < 8; i++)
		if (octree->children[i])
			Octree_Print(octree->children[i], level + 1);
	printf("%s}\n", l);
}