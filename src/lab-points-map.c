#include "../include/CORE.h"
#include "../include/labyrinth.h"

/*****************************************************************************************************************************/

#define LABPOINTSMAP_DEFAULTCAPACITY 	(40)

/*****************************************************************************************************************************/

typedef struct LabPointsMapStruct
{
	CORE_Handle 	CORE_ClassHandle;

	CORE_Handle 	*LabPointsArray;
	uint32 			Size;
	uint32 			Capacity;
} LabPointsMapStruct;

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

static void LabPointsMap_InternalPrint(CORE_Handle LabPointsMapHandle)
{
	LabPointsMapStruct 	*LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);
	CORE_Handle 		CurrentLabPointHandle, TempConnectionHandle;
	uint32 				TempConnectionID;


	CORE_DebugPrint("*** MAP START ***\n");
	for (uint32 i = 0; i < LabPointsMap->Size; i++)
		{
		LabPointsMap_GetPointByID(LabPointsMapHandle, i + 1, &CurrentLabPointHandle);
		LabPoint_GetID(CurrentLabPointHandle, &TempConnectionID);
		CORE_DebugPrint("[%ld]", TempConnectionID);

		LabPoint_GetConnectionTop(CurrentLabPointHandle, &TempConnectionHandle);
		if (TempConnectionHandle != NULL)
			{
			LabPoint_GetID(TempConnectionHandle, &TempConnectionID);
			CORE_DebugPrint(" %ld (top)", TempConnectionID);
			}

		LabPoint_GetConnectionRight(CurrentLabPointHandle, &TempConnectionHandle);
		if (TempConnectionHandle != NULL)
			{
			LabPoint_GetID(TempConnectionHandle, &TempConnectionID);
			CORE_DebugPrint(" %ld (right)", TempConnectionID);
			}

		LabPoint_GetConnectionBottom(CurrentLabPointHandle, &TempConnectionHandle);
		if (TempConnectionHandle != NULL)
			{
			LabPoint_GetID(TempConnectionHandle, &TempConnectionID);
			CORE_DebugPrint(" %ld (bottom)", TempConnectionID);
			}

		LabPoint_GetConnectionLeft(CurrentLabPointHandle, &TempConnectionHandle);
		if (TempConnectionHandle != NULL)
			{
			LabPoint_GetID(TempConnectionHandle, &TempConnectionID);
			CORE_DebugPrint(" %ld (left)", TempConnectionID);
			}

		CORE_DebugPrint("\n");
		}
	CORE_DebugPrint("*** MAP END ***\n");
}

static CORE_Bool LabPointsMap_InternalLabPointToRawLabPoint(CORE_Handle LabPointHandle, LabPointRawStruct *LabPointRaw)
{
	uint32 ID, TopID, RightID, BottomID, LeftID;
	CORE_Handle ConnectionHandle;


	ID = TopID = RightID = BottomID = LeftID = 0;
	LabPoint_GetID(LabPointHandle, &ID);

	LabPoint_GetConnectionTop(LabPointHandle, &ConnectionHandle);
	if (ConnectionHandle != NULL)
		LabPoint_GetID(ConnectionHandle, &TopID);

	LabPoint_GetConnectionRight(LabPointHandle, &ConnectionHandle);
	if (ConnectionHandle != NULL)
		LabPoint_GetID(ConnectionHandle, &RightID);

	LabPoint_GetConnectionBottom(LabPointHandle, &ConnectionHandle);
	if (ConnectionHandle != NULL)
		LabPoint_GetID(ConnectionHandle, &BottomID);

	LabPoint_GetConnectionLeft(LabPointHandle, &ConnectionHandle);
	if (ConnectionHandle != NULL)
		LabPoint_GetID(ConnectionHandle, &LeftID);

	LabPoint_GetIsExit(LabPointHandle, &LabPointRaw->IsExit);
	LabPoint_GetIsSpawn(LabPointHandle, &LabPointRaw->IsSpawn);

	LabPointRaw->ID = ID;
	LabPointRaw->ConnectionTopPointID = TopID;
	LabPointRaw->ConnectionRightPointID = RightID;
	LabPointRaw->ConnectionBottomPointID = BottomID;
	LabPointRaw->ConnectionLeftPointID = LeftID;

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool LabPointsMap_ToJSON(CORE_Handle LabPointsMapHandle, char **JSON)
{
	#define RAW_JSON_OBJECT_MAX_SIZE 	(120)
	#define DEC_JSON_LEFT(SIZE_TO_DEC) (JSONSizeLeft = (SIZE_TO_DEC > JSONSizeLeft ? 0 : (JSONSizeLeft - SIZE_TO_DEC))) 

	LabPointsMapStruct 	*LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);
	int32 				JSONSizeLeft;
	uint32 				MaxJSON;
	LabPointRawStruct 	LabPointRaw;
	char 				LabPointRawJSONObject[RAW_JSON_OBJECT_MAX_SIZE];
	uint32 				LabPointRawJSONObject_CharCount;


	#ifdef CORE_DEBUGENABLED
	LabPointsMap_InternalPrint(LabPointsMapHandle);
	#endif

	MaxJSON = LabPointsMap->Size * RAW_JSON_OBJECT_MAX_SIZE + 40;
	JSONSizeLeft = MaxJSON;
	*JSON = CORE_MemAlloc(sizeof(char) * MaxJSON + 1);
	*JSON[0] = '\0';

	LabPointRawJSONObject_CharCount = CORE_StringPrint(LabPointRawJSONObject, "{\"count\": %ld,\"points\": {", LabPointsMap->Size);
	CORE_StringCatLength(*JSON, LabPointRawJSONObject, JSONSizeLeft);
	DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

	for (uint32 i = 0; i < LabPointsMap->Size; i++)
		{
		LabPointsMap_InternalLabPointToRawLabPoint(LabPointsMap->LabPointsArray[i], &LabPointRaw);

		LabPointRawJSONObject_CharCount = CORE_StringPrint(LabPointRawJSONObject, "\"%ld\": ", LabPointRaw.ID);
		CORE_StringCatLength(*JSON, LabPointRawJSONObject, JSONSizeLeft);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		LabPointRawJSONObject_CharCount = CORE_StringPrint(LabPointRawJSONObject, 
															"{\"top_id\": %ld, \"right_id\": %ld, \"bottom_id\": %ld, \"left_id\": %ld, \"is_exit\": %ld, \"is_spawn\": %ld}",
															LabPointRaw.ConnectionTopPointID, LabPointRaw.ConnectionRightPointID, 
															LabPointRaw.ConnectionBottomPointID, LabPointRaw.ConnectionLeftPointID,
															LabPointRaw.IsExit, LabPointRaw.IsSpawn);

		CORE_StringCatLength(*JSON, LabPointRawJSONObject, JSONSizeLeft);
		DEC_JSON_LEFT(LabPointRawJSONObject_CharCount);

		if (i != LabPointsMap->Size - 1)
			{
			CORE_StringCatLength(*JSON, ",", JSONSizeLeft);
			DEC_JSON_LEFT(1);
			}
		}

	CORE_StringCatLength(*JSON, "}}", JSONSizeLeft);
	DEC_JSON_LEFT(2);

	return TRUE;
}

CORE_Bool LabPointsMap_ToRawData(CORE_Handle LabPointsMapHandle, uint8 **RawData, uint32 *RawDataSize)
{
	LabPointsMapStruct 	*LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);
	LabPointRawStruct 	*LabPointsRawData;


	*RawDataSize = sizeof(LabPointRawStruct) * LabPointsMap->Size;
	LabPointsRawData = CORE_MemAlloc(*RawDataSize);

	for (uint32 i = 0; i < LabPointsMap->Size; i++)
		{
		LabPointsMap_InternalLabPointToRawLabPoint(LabPointsMap->LabPointsArray[i], &LabPointsRawData[i]);
		}

	*RawData = (uint8 *) LabPointsRawData;

	return TRUE;
}

CORE_Bool LabPointsMap_AddPoint(CORE_Handle LabPointsMapHandle, CORE_Handle LabPointHandle)
{
	LabPointsMapStruct *LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);
	uint32 LabPointID;


	if (LabPoint_GetID(LabPointHandle, &LabPointID) == FALSE)
		return FALSE;

	if (LabPointsMap->Size == LabPointsMap->Capacity)
		{
		LabPointsMap->Capacity += LabPointsMap->Capacity;
		LabPointsMap->LabPointsArray = CORE_MemRealloc(LabPointsMap->LabPointsArray, sizeof(CORE_Handle) * LabPointsMap->Capacity);
		}

	LabPointsMap->LabPointsArray[LabPointID - 1] = LabPointHandle;
	LabPointsMap->Size++;

	return TRUE;
}

CORE_Bool LabPointsMap_GetPointByID(CORE_Handle LabPointsMapHandle, uint32 ID, CORE_Handle *LabPointHandle)
{
	LabPointsMapStruct *LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);


	*LabPointHandle = LabPointsMap->LabPointsArray[ID - 1];

	return TRUE;
}

CORE_Bool LabPointsMap_GetSize(CORE_Handle LabPointsMapHandle, uint32 *Size)
{
	LabPointsMapStruct *LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);


	*Size = LabPointsMap->Size;

	return TRUE;
}

CORE_Bool LabPointsMap_GetCapacity(CORE_Handle LabPointsMapHandle, uint32 *Capacity)
{
	LabPointsMapStruct *LabPointsMap = CORE_ClassCast(LabPointsMapHandle, LabPointsMapStruct);


	*Capacity = LabPointsMap->Capacity;

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool LabPointsMap_InternalCreate(LabPointsMapStruct *LabPointsMap)
{
	LabPointsMap->Capacity = LABPOINTSMAP_DEFAULTCAPACITY;
	LabPointsMap->Size = 0;
	LabPointsMap->LabPointsArray = CORE_MemAlloc(sizeof(CORE_Handle) * LabPointsMap->Capacity);

	return TRUE;
}

CORE_Bool LabPointsMap_InternalDelete(LabPointsMapStruct *LabPointsMap)
{
	CORE_MemFree(LabPointsMap->LabPointsArray);

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_ClassMakeConstructor(LabPointsMap);
CORE_ClassMakeDestructor(LabPointsMap);

/*****************************************************************************************************************************/
