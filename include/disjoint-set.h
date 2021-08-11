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

CORE_Bool DisjointSet_Print(CORE_Handle DisjointSetHandle);
CORE_Bool DisjointSet_Union(CORE_Handle DisjointSetHandle, int32 Value1, int32 Value2);
CORE_Bool DisjointSet_GetSubsetsCount(CORE_Handle DisjointSetHandle, uint32 *SubsetsCount);

CORE_Bool DisjointSet_Setup(CORE_Handle DisjointSetHandle, uint32 Size);

CORE_Bool DisjointSet_Create(CORE_Handle *DisjointSetHandle);
CORE_Bool DisjointSet_Delete(CORE_Handle *DisjointSetHandle);

#endif