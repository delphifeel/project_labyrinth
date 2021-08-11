#include "../include/CORE.h"
#include "../include/labyrinth.h"

/*****************************************************************************************************************************/

typedef struct ConnectionsStruct
{
	CORE_Handle 	TopLabPointHandle;
	CORE_Handle 	RightLabPointHandle;
	CORE_Handle 	BottomLabPointHandle;
	CORE_Handle 	LeftLabPointHandle;
} ConnectionsStruct;

typedef struct LabPointStruct
{
	CORE_Handle 		CORE_ClassHandle;

	uint32 				ID;
	ConnectionsStruct 	*Connections;
	CORE_Bool 			IsExit;
	CORE_Bool 			IsSpawn;
} LabPointStruct;

/*****************************************************************************************************************************/

CORE_Bool LabPoint_SetID(CORE_Handle LabPointHandle, uint32 ID)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	LabPoint->ID = ID;
	return TRUE;
}

CORE_Bool LabPoint_GetID(CORE_Handle LabPointHandle, uint32 *ID)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*ID = LabPoint->ID;
	return TRUE;
}

CORE_Bool LabPoint_SetConnectionTop(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	if (ConnectionLabPointHandle != NULL)
		CORE_ClassCheck(ConnectionLabPointHandle, LabPointStruct);

	LabPoint->Connections->TopLabPointHandle = ConnectionLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_GetConnectionTop(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*ConnectionLabPointHandle = LabPoint->Connections->TopLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_SetConnectionRight(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	if (ConnectionLabPointHandle != NULL)
		CORE_ClassCheck(ConnectionLabPointHandle, LabPointStruct);

	LabPoint->Connections->RightLabPointHandle = ConnectionLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_GetConnectionRight(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*ConnectionLabPointHandle = LabPoint->Connections->RightLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_SetConnectionBottom(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	if (ConnectionLabPointHandle != NULL)
		CORE_ClassCheck(ConnectionLabPointHandle, LabPointStruct);

	LabPoint->Connections->BottomLabPointHandle = ConnectionLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_GetConnectionBottom(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*ConnectionLabPointHandle = LabPoint->Connections->BottomLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_SetConnectionLeft(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	if (ConnectionLabPointHandle != NULL)
		CORE_ClassCheck(ConnectionLabPointHandle, LabPointStruct);
	
	LabPoint->Connections->LeftLabPointHandle = ConnectionLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_GetConnectionLeft(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*ConnectionLabPointHandle = LabPoint->Connections->LeftLabPointHandle;
	return TRUE;
}

CORE_Bool LabPoint_SetIsExit(CORE_Handle LabPointHandle, CORE_Bool Value)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	LabPoint->IsExit = Value;
	return TRUE;
}

CORE_Bool LabPoint_GetIsExit(CORE_Handle LabPointHandle, CORE_Bool *Value)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*Value = LabPoint->IsExit;
	return TRUE;
}

CORE_Bool LabPoint_SetIsSpawn(CORE_Handle LabPointHandle, CORE_Bool Value)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	LabPoint->IsSpawn = Value;
	return TRUE;
}

CORE_Bool LabPoint_GetIsSpawn(CORE_Handle LabPointHandle, CORE_Bool *Value)
{
	LabPointStruct *LabPoint = CORE_ClassCast(LabPointHandle, LabPointStruct);


	*Value = LabPoint->IsSpawn;
	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool LabPoint_InternalCreate(LabPointStruct *LabPoint)
{
	ConnectionsStruct *Connections = CORE_MemAlloc(sizeof(ConnectionsStruct));
	Connections->TopLabPointHandle = NULL;
	Connections->RightLabPointHandle = NULL;
	Connections->BottomLabPointHandle = NULL;
	Connections->LeftLabPointHandle = NULL;

	LabPoint->Connections = Connections;

	LabPoint->IsExit = FALSE;
	LabPoint->IsSpawn = FALSE;

	return TRUE;
}

CORE_Bool LabPoint_InternalDelete(LabPointStruct *LabPoint)
{
	CORE_MemFree(LabPoint->Connections);

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_ClassMakeConstructor(LabPoint);
CORE_ClassMakeDestructor(LabPoint);

/*****************************************************************************************************************************/