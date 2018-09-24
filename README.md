# C-Octree
==============
This is a C implementation of an AABB Octree. Simply include the header files and compile/link the c files. It uses no external libraries. Each function takes an octree to perform an operation on, and in most cases two float vectors with 3 components (vec3). The only dependency on linmath.h is the definition of vec3, so it can be easily removed. I use it in my projects because its definitions match up nicely with GLSL.

Most of the code is self explanitory. Here are some tips:

- The second parameter on the print function is to set the level of indentation. I enjoy using "|-" repeated to make my indentation because it is easier to keep track of the level of indentation.
- Create an octree with a call to Octree_New. The size (second parameter) is specified as a power of two. 4 means that your octree will end up being 16 units long in each dimension and centered at the position you designate (the first parameter). Note that it can still accept AABB's outside this range, but they will all be placed in the outermost Octree-Cells (not very efficient space usage).
- Don't forget to call Octree_Free when you are done. This will recursively free memory for dynamically allocated Octree-Cells.
- The two points passed into each function don't have to be the minimum xyz and maximum xyz corners. The Octree checks the x, y, and z components on both to make sure they are in the same Octree-Cell, so it doesn't matter if the minimum in each dimension are all stored in one point and likewise for the maximum.
- An Octree intersection doesn't mean that the AABB's intersect. It only means that they lie inthe same Octree-Cell. Note that a badly placed but small AABB may end up being stored in the root of the octree and show up in every intersection test. A AABB intersection test needs to be run after performing the Octree intersection. I may end up building this into the Octree in the future.
- The LinkedList implementation's stores data in nodes after the first. This simplifies some opperations. The first just exists so the LinkedList has a location in memory that can be passed around even while empty.

