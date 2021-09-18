#include "CORE/disjoint-set.h"

int main()
{
	DisjointSet disjoint_set_handle;


	DisjointSet_Create(&disjoint_set_handle);
	DisjointSet_Setup(disjoint_set_handle, 10);

	DisjointSet_Union(disjoint_set_handle, 1, 2);
	DisjointSet_Union(disjoint_set_handle, 3, 1);
	DisjointSet_Union(disjoint_set_handle, 4, 5);
	DisjointSet_Union(disjoint_set_handle, 2, 5);

	DisjointSet_Print(disjoint_set_handle);

	DisjointSet_Free(&disjoint_set_handle);
}
