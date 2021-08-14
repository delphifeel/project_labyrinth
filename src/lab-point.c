#include "../include/CORE.h"
#include "../include/labyrinth/lab-point.h"


void LabPoint_SetID(LabPoint Instance, uint32 ID)
{
	Instance->ID = ID;
}

void LabPoint_GetID(LabPoint Instance, uint32 *ID)
{
	*ID = Instance->ID;
}

void LabPoint_SetConnectionTop(LabPoint Instance, LabPoint ConnectionLabPointHandle)
{
	Instance->Connections->TopLabPointHandle = ConnectionLabPointHandle;
}

void LabPoint_GetConnectionTop(LabPoint Instance, LabPoint *ConnectionLabPointHandle)
{
	*ConnectionLabPointHandle = Instance->Connections->TopLabPointHandle;
}

void LabPoint_SetConnectionRight(LabPoint Instance, LabPoint ConnectionLabPointHandle)
{
	Instance->Connections->RightLabPointHandle = ConnectionLabPointHandle;
}

void LabPoint_GetConnectionRight(LabPoint Instance, LabPoint *ConnectionLabPointHandle)
{
	*ConnectionLabPointHandle = Instance->Connections->RightLabPointHandle;
}

void LabPoint_SetConnectionBottom(LabPoint Instance, LabPoint ConnectionLabPointHandle)
{
	Instance->Connections->BottomLabPointHandle = ConnectionLabPointHandle;
}

void LabPoint_GetConnectionBottom(LabPoint Instance, LabPoint *ConnectionLabPointHandle)
{
	*ConnectionLabPointHandle = Instance->Connections->BottomLabPointHandle;
}

void LabPoint_SetConnectionLeft(LabPoint Instance, LabPoint ConnectionLabPointHandle)
{
	Instance->Connections->LeftLabPointHandle = ConnectionLabPointHandle;
}

void LabPoint_GetConnectionLeft(LabPoint Instance, LabPoint *ConnectionLabPointHandle)
{
	*ConnectionLabPointHandle = Instance->Connections->LeftLabPointHandle;
}

void LabPoint_SetIsExit(LabPoint Instance, CORE_Bool Value)
{
	Instance->IsExit = Value;
}

void LabPoint_GetIsExit(LabPoint Instance, CORE_Bool *Value)
{
	*Value = Instance->IsExit;
}

void LabPoint_SetIsSpawn(LabPoint Instance, CORE_Bool Value)
{
	Instance->IsSpawn = Value;
}

void LabPoint_GetIsSpawn(LabPoint Instance, CORE_Bool *Value)
{
	*Value = Instance->IsSpawn;
}

/*****************************************************************************************************************************/

void LabPoint_Create(LabPoint *InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, LabPoint);

	(*InstancePtr)->Connections = CORE_MemAlloc(sizeof(ConnectionsStruct));
	
	(*InstancePtr)->Connections->TopLabPointHandle = NULL;
	(*InstancePtr)->Connections->RightLabPointHandle = NULL;
	(*InstancePtr)->Connections->BottomLabPointHandle = NULL;
	(*InstancePtr)->Connections->LeftLabPointHandle = NULL;

	(*InstancePtr)->IsExit = FALSE;
	(*InstancePtr)->IsSpawn = FALSE;
}

void LabPoint_Free(LabPoint *InstancePtr)
{
	CORE_MemFree((*InstancePtr)->Connections);

	CORE_OBJECT_FREE(InstancePtr);
}

