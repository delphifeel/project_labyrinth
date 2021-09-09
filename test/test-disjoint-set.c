#include "../include/disjoint-set.h"

int main()
{
	DisjointSet DisjointSetHandle;


	DisjointSet_Create(&DisjointSetHandle);
	DisjointSet_Setup(DisjointSetHandle, 10);

	DisjointSet_Union(DisjointSetHandle, 1, 2);
	DisjointSet_Union(DisjointSetHandle, 3, 1);
	DisjointSet_Union(DisjointSetHandle, 4, 5);
	DisjointSet_Union(DisjointSetHandle, 2, 5);

	DisjointSet_Print(DisjointSetHandle);

	DisjointSet_Free(&DisjointSetHandle);
}