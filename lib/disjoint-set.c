#include "../include/disjoint-set.h"

CORE_OBJECT_INTERFACE(DisjointSet,
	int32 			*MapChildToParent;
	uint32 			MapChildToParentSize;
	uint32 			SubsetsCount;
);

/*****************************************************************************************************************************/

static void DisjointSet_InternalFindSubset(DisjointSet Instance, int32 Value, int32 *Subset)
{
	int32 Parent;


	if ((Parent = Instance->MapChildToParent[Value]) != -1)
		DisjointSet_InternalFindSubset(Instance, Parent, Subset);
	else
		*Subset = Value;
}

/*****************************************************************************************************************************/

void DisjointSet_Print(DisjointSet Instance)
{
	uint8 				ProcessedValues[Instance->MapChildToParentSize];
	int32 				CurrentParent;


	CORE_MemSet(ProcessedValues, 0, sizeof(ProcessedValues));

	CORE_DebugPrint("DisjoinSet:\n");
	CORE_DebugPrint("Subsets Count: %ld\n", Instance->SubsetsCount);

	for (uint32 i = 0; i < Instance->MapChildToParentSize; i++)
		{
		if (ProcessedValues[i] == 1)
			continue;

		CurrentParent = i;
		ProcessedValues[CurrentParent] = 1;

		CORE_DebugPrint("{");
		while (Instance->MapChildToParent[CurrentParent] != -1)
			{
			CORE_DebugPrint("%ld, ", CurrentParent);
			CurrentParent = Instance->MapChildToParent[CurrentParent];
			ProcessedValues[CurrentParent] = 1;
			}

		CORE_DebugPrint("%ld} [%ld] \n", CurrentParent, CurrentParent);
		}
}

CORE_Bool DisjointSet_Union(DisjointSet Instance, int32 Value1, int32 Value2)
{
	int32 Value1Subset, Value2Subset;


	DisjointSet_InternalFindSubset(Instance, Value1, &Value1Subset);
	DisjointSet_InternalFindSubset(Instance, Value2, &Value2Subset);

	if (Value1Subset == Value2Subset)
		return FALSE;

	Instance->MapChildToParent[Value1Subset] = Value2Subset;
	Instance->SubsetsCount--;

	return TRUE;
}

void DisjointSet_GetSubsetsCount(DisjointSet Instance, uint32 *SubsetsCount)
{
	*SubsetsCount = Instance->SubsetsCount;
}

/*****************************************************************************************************************************/

void DisjointSet_Setup(DisjointSet Instance, uint32 Size)
{
	Instance->MapChildToParentSize = Size;
	Instance->SubsetsCount = Size;
	Instance->MapChildToParent = CORE_MemAlloc(sizeof(int32) * Size);
	CORE_MemSet(Instance->MapChildToParent, -1, sizeof(int32) * Size);
}

/*****************************************************************************************************************************/

void DisjointSet_Create(DisjointSet *InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, DisjointSet);

	(*InstancePtr)->MapChildToParentSize = 0;
	(*InstancePtr)->MapChildToParent = NULL;
	(*InstancePtr)->SubsetsCount = 0;
}

void DisjointSet_Free(DisjointSet *InstancePtr)
{
	CORE_MemFree((*InstancePtr)->MapChildToParent);
	CORE_OBJECT_FREE(InstancePtr);
}

/*****************************************************************************************************************************/
