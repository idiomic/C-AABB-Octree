#include "LinkedList.h"
#include "linmath.h"
#include <stdio.h>

#define MINIMUM_OCTREE_CELL_SIZE 1.0f
#define CREATE 0
#define NO_CREATE 1

struct Octree_Cell {
	struct Octree_Cell* parent;
	float x;
	float y;
	float z;
	int size;
	LinkedList values;
	struct Octree_Cell* children[8];
};

typedef struct Octree_Cell* Octree;

Octree Octree_New(vec3, int);
Octree Octree_GetChild(Octree, vec3, vec3, int);
Octree Octree_GetDescendent(Octree, vec3, vec3, int);
void Octree_Insert(Octree, vec3, vec3, void*);
void Octree_Remove(Octree, vec3, vec3, void*);
void Octree_Append(Octree, LinkedList);
LinkedList Octree_Intersection(Octree, vec3, vec3);
void Octree_Free(Octree);
void Octree_Print(Octree, int);