#ifndef __LAB_POINT_H__
#define __LAB_POINT_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

typedef struct ConnectionsStruct ConnectionsStruct;

CORE_OBJECT_INTERFACE(LabPoint,
	uint32 				ID;
	ConnectionsStruct 	*Connections;
	CORE_Bool 			IsExit;
	CORE_Bool 			IsSpawn;
)

struct ConnectionsStruct
{
	LabPoint 	TopLabPointHandle;
	LabPoint 	RightLabPointHandle;
	LabPoint 	BottomLabPointHandle;
	LabPoint 	LeftLabPointHandle;
};

/*****************************************************************************************************************************/

void LabPoint_SetID(LabPoint, uint32 ID);
void LabPoint_SetConnectionTop(LabPoint, LabPoint ConnectionLabPoint);
void LabPoint_SetConnectionRight(LabPoint, LabPoint ConnectionLabPoint);
void LabPoint_SetConnectionBottom(LabPoint, LabPoint ConnectionLabPoint);
void LabPoint_SetConnectionLeft(LabPoint, LabPoint ConnectionLabPoint);
void LabPoint_SetIsExit(LabPoint, CORE_Bool Value);
void LabPoint_SetIsSpawn(LabPoint, CORE_Bool Value);

void LabPoint_GetID(LabPoint, uint32 *ID);
void LabPoint_GetConnectionTop(LabPoint, LabPoint *ConnectionLabPoint);
void LabPoint_GetConnectionRight(LabPoint, LabPoint *ConnectionLabPoint);
void LabPoint_GetConnectionBottom(LabPoint, LabPoint *ConnectionLabPoint);
void LabPoint_GetConnectionLeft(LabPoint, LabPoint *ConnectionLabPoint);
void LabPoint_GetIsExit(LabPoint, CORE_Bool *Value);
void LabPoint_GetIsSpawn(LabPoint, CORE_Bool *Value);

void LabPoint_Create(LabPoint*);
void LabPoint_Free(LabPoint*);

/*****************************************************************************************************************************/

#endif