/*
	Disjoint set.
	Created from numbers according to setup size.

	Example: 
		for size 10 will be created 10 sets
		{0} {1} {2} {3} {4} {5} {6} {7} {8} {9}
*/

#ifndef __DISJOINT_SET_H__
#define __DISJOINT_SET_H__

#include "./CORE.h"

CORE_OBJECT_INTERFACE(DisjointSet,
	int32 			*MapChildToParent;
	uint32 			MapChildToParentSize;
	uint32 			SubsetsCount;
)

void 		DisjointSet_Print(DisjointSet);
CORE_Bool 	DisjointSet_Union(DisjointSet, int32 Value1, int32 Value2);
void 		DisjointSet_GetSubsetsCount(DisjointSet, uint32 *SubsetsCount);

void 		DisjointSet_Setup(DisjointSet, uint32 Size);

void 		DisjointSet_Create(DisjointSet*);
void 		DisjointSet_Free(DisjointSet*);

#endif
