#include <string.h>

#include "../include/CORE.h"

#include "../include/labyrinth/lab-point.h"
#include "../include/labyrinth/lab-points-map.h"

/*****************************************************************************************************************************/

#define LABPOINTSMAP_DEFAULTCAPACITY 	(40)

/*****************************************************************************************************************************/

static void LabPointsMap_InternalPrint(LabPointsMap Instance)
{
	LabPointStruct 		CurrentLabPoint;

	CORE_DebugPrint("*** MAP START ***\n");
	for (uint32 i = 0; i < Instance->Size; i++)
	{
		
		LabPointsMap_GetPointByID(Instance, i + 1, &CurrentLabPoint);
		CORE_DebugPrint("[%ld]", CurrentLabPoint.Id);
		CORE_DebugPrint(" %ld (top)", CurrentLabPoint.TopConnectionId);
		CORE_DebugPrint(" %ld (right)", CurrentLabPoint.RightConnectionId);
		CORE_DebugPrint(" %ld (bottom)", CurrentLabPoint.BottomConnectionId);
		CORE_DebugPrint(" %ld (left)", CurrentLabPoint.LeftConnectionId);
		CORE_DebugPrint("\n");
	}
	CORE_DebugPrint("*** MAP END ***\n");
}

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap Instance, char **JSON)
{
	#define RAW_JSON_OBJECT_MAX_SIZE 	(120)
	#define DEC_JSON_LEFT(SIZE_TO_DEC) (JSONSizeLeft = (SIZE_TO_DEC > JSONSizeLeft ? 0 : (JSONSizeLeft - SIZE_TO_DEC))) 

	int32 				JSONSizeLeft;
	uint32 				MaxJSON;
	LabPointStruct		LabPoint; 
	char 				LabPointRawJSONObject[RAW_JSON_OBJECT_MAX_SIZE];
	uint32 				LabPointRawJSONObject_CharCount;


	#ifdef CORE_DEBUGENABLED
	LabPointsMap_InternalPrint(Instance);
	#endif

	MaxJSON = Instance->Size * RAW_JSON_OBJECT_MAX_SIZE + 40;
	JSONSizeLeft = MaxJSON;
	*JSON = CORE_MemAlloc(sizeof(char) * MaxJSON + 1);
	*JSON[0] = '\0';

	LabPointRawJSONObject_CharCount = sprintf(LabPointRawJSONObject, "{\"count\": %ld,\"points\": {", Instance->Size);
	strncat(*JSON, LabPointRawJSONObject, LabPointRawJSONObject_CharCount);
	DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

	for (uint32 i = 0; i < Instance->Size; i++)
	{
		
		LabPoint = Instance->LabPointsArray[i];
		LabPointRawJSONObject_CharCount = sprintf(LabPointRawJSONObject, "\"%ld\": ", LabPoint.Id);
		strncat(*JSON, LabPointRawJSONObject, LabPointRawJSONObject_CharCount);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		LabPointRawJSONObject_CharCount = sprintf(LabPointRawJSONObject, 
															"{\"top_id\": %ld, \"right_id\": %ld, \"bottom_id\": %ld, \"left_id\": %ld, \"is_exit\": %ld, \"is_spawn\": %ld}",
															LabPoint.TopConnectionId, LabPoint.RightConnectionId, 
															LabPoint.BottomConnectionId, LabPoint.LeftConnectionId,
															LabPoint.IsExit, LabPoint.IsSpawn);

		strncat(*JSON, LabPointRawJSONObject, LabPointRawJSONObject_CharCount);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		if (i != Instance->Size - 1)
		{
			strncat(*JSON, ",", 1);
			DEC_JSON_LEFT(1);
		}
	}

	strncat(*JSON, "}}", 2);
	DEC_JSON_LEFT(2);
}

void LabPointsMap_ToRawData(LabPointsMap Instance, uint8 **RawData, uint32 *RawDataSize)
{
	LabPointStruct 	*LabPointData;

	*RawDataSize = sizeof(LabPointStruct) * Instance->Size;
	LabPointData = CORE_MemAlloc(*RawDataSize);

	for (uint32 i = 0; i < Instance->Size; i++)
	{
		LabPointsMap_InternalLabPointToRawLabPoint(Instance->LabPointsArray[i], &LabPointData[i]);
	}

	*RawData = (uint8 *) LabPointData;
}

void LabPointsMap_AddPoint(LabPointsMap Instance, LabPointStruct Point)
{
	uint32 LabPointID;


	LabPoint_GetID(Point, &LabPointID);

	if (Instance->Size == Instance->Capacity)		// if TRUE, double capacity. 
	{
		Instance->Capacity += Instance->Capacity;
		Instance->LabPointsArray = CORE_MemRealloc(Instance->LabPointsArray, sizeof(LabPointStruct) * Instance->Capacity);
	}

	Instance->LabPointsArray[LabPointID - 1] = Point;
	Instance->Size++;
}

void LabPointsMap_GetPointByID(LabPointsMap Instance, uint32 ID, LabPointStruct *Point)
{
	*Point = Instance->LabPointsArray[ID - 1];
}

void LabPointsMap_GetSize(LabPointsMap Instance, uint32 *Size)
{
	*Size = Instance->Size;
}

void LabPointsMap_GetCapacity(LabPointsMap Instance, uint32 *Capacity)
{
	*Capacity = Instance->Capacity;
}

/*****************************************************************************************************************************/

void LabPointsMap_Create(LabPointsMap *InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, LabPointsMap);

	(*InstancePtr)->Capacity = LABPOINTSMAP_DEFAULTCAPACITY;
	(*InstancePtr)->Size = 0;
	(*InstancePtr)->LabPointsArray = NULL;
	(*InstancePtr)->LabPointsArray = CORE_MemAlloc(sizeof(LabPointStruct) * ((*InstancePtr)->Capacity));
}

void LabPointsMap_Free(LabPointsMap *InstancePtr)
{
	for (uint32 i = 0; i < (*InstancePtr)->Size; i++)
		{
		LabPoint_Free(&(*InstancePtr)->LabPointsArray[i]);
		}

	CORE_MemFree((*InstancePtr)->LabPointsArray);
	CORE_OBJECT_FREE(InstancePtr);
}

/*****************************************************************************************************************************/
