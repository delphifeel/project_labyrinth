/*
	Disjoint set.
	Created from numbers according to setup size.

	Example: 
		for size 10 will be created 10 sets
		{0} {1} {2} {3} {4} {5} {6} {7} {8} {9}
*/

#ifndef _DISJOINT_SET_H_
#define _DISJOINT_SET_H_

#include "../CORE.h"

CORE_OBJECT_DEFINE(DisjointSet);

void 		DisjointSet_Print(DisjointSet instance);
CORE_Bool 	DisjointSet_Union(DisjointSet instance, int32 value1, int32 value2);
void 		DisjointSet_GetSubsetsCount(DisjointSet instance, uint32 *subsets_count);

void 		DisjointSet_Setup(DisjointSet instance, uint32 size);

void 		DisjointSet_Create(DisjointSet* instance_ptr);
void 		DisjointSet_Free(DisjointSet* instance_ptr);

#endif
