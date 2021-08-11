#include "../include/disjoint-set.h"

/*****************************************************************************************************************************/

typedef struct DisjointSetStruct
{
	CORE_Handle 	CORE_ClassHandle;

	int32 			*MapChildToParent;
	uint32 			MapChildToParentSize;
	uint32 			SubsetsCount;
} DisjointSetStruct;

/*****************************************************************************************************************************/

static CORE_Bool DisjointSet_InternalFindSubset(CORE_Handle DisjointSetHandle, int32 Value, int32 *Subset)
{
	DisjointSetStruct *DisjointSet = CORE_ClassCast(DisjointSetHandle, DisjointSetStruct);
	int32 Parent;


	if ((Parent = DisjointSet->MapChildToParent[Value]) != -1)
		return DisjointSet_InternalFindSubset(DisjointSetHandle, Parent, Subset);
	else
		*Subset = Value;

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool DisjointSet_Print(CORE_Handle DisjointSetHandle)
{
	DisjointSetStruct 	*DisjointSet = CORE_ClassCast(DisjointSetHandle, DisjointSetStruct);
	uint8 				ProcessedValues[DisjointSet->MapChildToParentSize];
	int32 				CurrentParent;


	CORE_MemSet(ProcessedValues, 0, sizeof(ProcessedValues));

	CORE_DebugPrint("DisjoinSet:\n");
	CORE_DebugPrint("Subsets Count: %ld\n", DisjointSet->SubsetsCount);

	for (uint32 i = 0; i < DisjointSet->MapChildToParentSize; i++)
		{
		if (ProcessedValues[i] == 1)
			continue;

		CurrentParent = i;
		ProcessedValues[CurrentParent] = 1;

		CORE_DebugPrint("{");
		while (DisjointSet->MapChildToParent[CurrentParent] != -1)
			{
			CORE_DebugPrint("%ld, ", CurrentParent);
			CurrentParent = DisjointSet->MapChildToParent[CurrentParent];
			ProcessedValues[CurrentParent] = 1;
			}

		CORE_DebugPrint("%ld} [%ld] \n", CurrentParent, CurrentParent);
		}

	return TRUE;
}

CORE_Bool DisjointSet_Union(CORE_Handle DisjointSetHandle, int32 Value1, int32 Value2)
{
	DisjointSetStruct *DisjointSet = CORE_ClassCast(DisjointSetHandle, DisjointSetStruct);
	int32 Value1Subset, Value2Subset;


	DisjointSet_InternalFindSubset(DisjointSetHandle, Value1, &Value1Subset);
	DisjointSet_InternalFindSubset(DisjointSetHandle, Value2, &Value2Subset);

	if (Value1Subset == Value2Subset)
		return FALSE;

	DisjointSet->MapChildToParent[Value1Subset] = Value2Subset;

	DisjointSet->SubsetsCount--;

	return TRUE;
}

CORE_Bool DisjointSet_GetSubsetsCount(CORE_Handle DisjointSetHandle, uint32 *SubsetsCount)
{
	DisjointSetStruct *DisjointSet = CORE_ClassCast(DisjointSetHandle, DisjointSetStruct);


	*SubsetsCount = DisjointSet->SubsetsCount;
	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool DisjointSet_Setup(CORE_Handle DisjointSetHandle, uint32 Size)
{
	DisjointSetStruct *DisjointSet = CORE_ClassCast(DisjointSetHandle, DisjointSetStruct);


	DisjointSet->MapChildToParentSize = Size;
	DisjointSet->SubsetsCount = Size;
	DisjointSet->MapChildToParent = CORE_MemAlloc(sizeof(int32) * Size);
	CORE_MemSet(DisjointSet->MapChildToParent, -1, sizeof(int32) * Size);

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool DisjointSet_InternalCreate(DisjointSetStruct *DisjointSet)
{
	DisjointSet->MapChildToParentSize = 0;
	DisjointSet->MapChildToParent = NULL;
	DisjointSet->SubsetsCount = 0;

	return TRUE;
}

CORE_Bool DisjointSet_InternalDelete(DisjointSetStruct *DisjointSet)
{
	CORE_MemFree(DisjointSet->MapChildToParent);

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_ClassMakeConstructor(DisjointSet);
CORE_ClassMakeDestructor(DisjointSet);

/*****************************************************************************************************************************/
