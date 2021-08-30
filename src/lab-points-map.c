#include <string.h>

#include "../include/CORE.h"

#include "../include/labyrinth/lab-point.h"
#include "../include/labyrinth/lab-points-map.h"

/*****************************************************************************************************************************/

#define LABPOINTSMAP_DEFAULTCAPACITY 	(40)

/*****************************************************************************************************************************/



typedef struct LabPointRawStruct
{
	uint32 		ID;
	uint32 		ConnectionTopPointID;
	uint32 		ConnectionRightPointID;
	uint32 		ConnectionBottomPointID;
	uint32 		ConnectionLeftPointID;
	CORE_Bool 	IsExit;
	CORE_Bool 	IsSpawn;
} LabPointRawStruct;


/*****************************************************************************************************************************/

static void LabPointsMap_InternalPrint(LabPointsMap Instance)
{
	LabPoint 		CurrentLabPoint, TempConnection;
	uint32 				TempConnectionID;


	CORE_DebugPrint("*** MAP START ***\n");
	for (uint32 i = 0; i < Instance->Size; i++)
		{
		LabPointsMap_GetPointByID(Instance, i + 1, &CurrentLabPoint);
		LabPoint_GetID(CurrentLabPoint, &TempConnectionID);
		CORE_DebugPrint("[%ld]", TempConnectionID);

		LabPoint_GetConnectionTop(CurrentLabPoint, &TempConnection);
		if (TempConnection != NULL)
			{
			LabPoint_GetID(TempConnection, &TempConnectionID);
			CORE_DebugPrint(" %ld (top)", TempConnectionID);
			}

		LabPoint_GetConnectionRight(CurrentLabPoint, &TempConnection);
		if (TempConnection != NULL)
			{
			LabPoint_GetID(TempConnection, &TempConnectionID);
			CORE_DebugPrint(" %ld (right)", TempConnectionID);
			}

		LabPoint_GetConnectionBottom(CurrentLabPoint, &TempConnection);
		if (TempConnection != NULL)
			{
			LabPoint_GetID(TempConnection, &TempConnectionID);
			CORE_DebugPrint(" %ld (bottom)", TempConnectionID);
			}

		LabPoint_GetConnectionLeft(CurrentLabPoint, &TempConnection);
		if (TempConnection != NULL)
			{
			LabPoint_GetID(TempConnection, &TempConnectionID);
			CORE_DebugPrint(" %ld (left)", TempConnectionID);
			}

		CORE_DebugPrint("\n");
		}
	CORE_DebugPrint("*** MAP END ***\n");
}

static void LabPointsMap_InternalLabPointToRawLabPoint(LabPoint Point, LabPointRawStruct *LabPointRaw)
{
	uint32 ID, TopID, RightID, BottomID, LeftID;
	LabPoint Connection;


	ID = TopID = RightID = BottomID = LeftID = 0;
	LabPoint_GetID(Point, &ID);

	LabPoint_GetConnectionTop(Point, &Connection);
	if (Connection != NULL)
		LabPoint_GetID(Connection, &TopID);

	LabPoint_GetConnectionRight(Point, &Connection);
	if (Connection != NULL)
		LabPoint_GetID(Connection, &RightID);

	LabPoint_GetConnectionBottom(Point, &Connection);
	if (Connection != NULL)
		LabPoint_GetID(Connection, &BottomID);

	LabPoint_GetConnectionLeft(Point, &Connection);
	if (Connection != NULL)
		LabPoint_GetID(Connection, &LeftID);

	LabPoint_GetIsExit(Point, &LabPointRaw->IsExit);
	LabPoint_GetIsSpawn(Point, &LabPointRaw->IsSpawn);

	LabPointRaw->ID = ID;
	LabPointRaw->ConnectionTopPointID = TopID;
	LabPointRaw->ConnectionRightPointID = RightID;
	LabPointRaw->ConnectionBottomPointID = BottomID;
	LabPointRaw->ConnectionLeftPointID = LeftID;
}

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap Instance, char **JSON)
{
	#define RAW_JSON_OBJECT_MAX_SIZE 	(120)
	#define DEC_JSON_LEFT(SIZE_TO_DEC) (JSONSizeLeft = (SIZE_TO_DEC > JSONSizeLeft ? 0 : (JSONSizeLeft - SIZE_TO_DEC))) 

	int32 				JSONSizeLeft;
	uint32 				MaxJSON;
	LabPointRawStruct 	LabPointRaw;
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
	strncat(*JSON, LabPointRawJSONObject, JSONSizeLeft);
	DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

	for (uint32 i = 0; i < Instance->Size; i++)
		{
		LabPointsMap_InternalLabPointToRawLabPoint(Instance->LabPointsArray[i], &LabPointRaw);

		LabPointRawJSONObject_CharCount = sprintf(LabPointRawJSONObject, "\"%ld\": ", LabPointRaw.ID);
		strncat(*JSON, LabPointRawJSONObject, JSONSizeLeft);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		LabPointRawJSONObject_CharCount = sprintf(LabPointRawJSONObject, 
															"{\"top_id\": %ld, \"right_id\": %ld, \"bottom_id\": %ld, \"left_id\": %ld, \"is_exit\": %ld, \"is_spawn\": %ld}",
															LabPointRaw.ConnectionTopPointID, LabPointRaw.ConnectionRightPointID, 
															LabPointRaw.ConnectionBottomPointID, LabPointRaw.ConnectionLeftPointID,
															LabPointRaw.IsExit, LabPointRaw.IsSpawn);

		strncat(*JSON, LabPointRawJSONObject, JSONSizeLeft);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		if (i != Instance->Size - 1)
			{
			strncat(*JSON, ",", JSONSizeLeft);
			DEC_JSON_LEFT(1);
			}
		}

	strncat(*JSON, "}}", JSONSizeLeft);
	DEC_JSON_LEFT(2);
}

void LabPointsMap_ToRawData(LabPointsMap Instance, uint8 **RawData, uint32 *RawDataSize)
{
	LabPointRawStruct 	*LabPointsRawData;


	*RawDataSize = sizeof(LabPointRawStruct) * Instance->Size;
	LabPointsRawData = CORE_MemAlloc(*RawDataSize);

	for (uint32 i = 0; i < Instance->Size; i++)
		{
		LabPointsMap_InternalLabPointToRawLabPoint(Instance->LabPointsArray[i], &LabPointsRawData[i]);
		}

	*RawData = (uint8 *) LabPointsRawData;
}

void LabPointsMap_AddPoint(LabPointsMap Instance, LabPoint Point)
{
	uint32 LabPointID;


	LabPoint_GetID(Point, &LabPointID);

	if (Instance->Size == Instance->Capacity)
		{
		Instance->Capacity += Instance->Capacity;
		Instance->LabPointsArray = CORE_MemRealloc(Instance->LabPointsArray, sizeof(LabPoint) * Instance->Capacity);
		}

	Instance->LabPointsArray[LabPointID - 1] = Point;
	Instance->Size++;
}

void LabPointsMap_GetPointByID(LabPointsMap Instance, uint32 ID, LabPoint *Point)
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
	(*InstancePtr)->LabPointsArray = CORE_MemAlloc(sizeof(LabPoint) * ((*InstancePtr)->Capacity));
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
