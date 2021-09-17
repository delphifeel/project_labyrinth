#include "../include/disjoint-set.h"

CORE_OBJECT_INTERFACE(DisjointSet,
	int32 			*map_child_to_parent;
	uint32 			map_child_to_parent_size;
	uint32 			subsets_count;
);

/*****************************************************************************************************************************/

static void DisjointSet_InternalFindSubset(DisjointSet instance, int32 value, int32 *subset)
{
	int32 parent;


	if ((parent = instance->map_child_to_parent[value]) != -1) 
	{
		DisjointSet_InternalFindSubset(instance, parent, subset);
	} 
	else 
	{
		*subset = value;
	}
}

/*****************************************************************************************************************************/

void DisjointSet_Print(DisjointSet instance)
{
	uint8 				processed_values[instance->map_child_to_parent_size];
	int32 				current_parent;


	CORE_MemSet(processed_values, 0, sizeof(processed_values));

	CORE_DebugPrint("DisjoinSet:\n");
	CORE_DebugPrint("Subsets Count: %ld\n", instance->subsets_count);

	for (uint32 i = 0; i < instance->map_child_to_parent_size; i++)
	{
		if (processed_values[i] == 1) 
		{
			continue;
		}

		current_parent = i;
		processed_values[current_parent] = 1;

		CORE_DebugPrint("{");
		while (instance->map_child_to_parent[current_parent] != -1)
		{
			CORE_DebugPrint("%ld, ", current_parent);
			current_parent = instance->map_child_to_parent[current_parent];
			processed_values[current_parent] = 1;
		}

		CORE_DebugPrint("%ld} [%ld] \n", current_parent, current_parent);
	}
}

CORE_Bool DisjointSet_Union(DisjointSet instance, int32 value1, int32 value2)
{
	int32 value1_subset, value2_subset;


	DisjointSet_InternalFindSubset(instance, value1, &value1_subset);
	DisjointSet_InternalFindSubset(instance, value2, &value2_subset);

	if (value1_subset == value2_subset) 
	{
		return FALSE;
	}

	instance->map_child_to_parent[value1_subset] = value2_subset;
	instance->subsets_count--;

	return TRUE;
}

void DisjointSet_GetSubsetsCount(DisjointSet instance, uint32 *subsets_count)
{
	*subsets_count = instance->subsets_count;
}

/*****************************************************************************************************************************/

void DisjointSet_Setup(DisjointSet instance, uint32 size)
{
	instance->map_child_to_parent_size = size;
	instance->subsets_count = size;
	instance->map_child_to_parent = CORE_MemAlloc(sizeof(int32) * size);
	CORE_MemSet(instance->map_child_to_parent, -1, sizeof(int32) * size);
}

/*****************************************************************************************************************************/

void DisjointSet_Create(DisjointSet *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, DisjointSet);

	(*instance_ptr)->map_child_to_parent_size = 0;
	(*instance_ptr)->map_child_to_parent = NULL;
	(*instance_ptr)->subsets_count = 0;
}

void DisjointSet_Free(DisjointSet *instance_ptr)
{
	CORE_MemFree((*instance_ptr)->map_child_to_parent);
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
