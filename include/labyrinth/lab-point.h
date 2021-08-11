#ifndef __LAB_POINT_H__
#define __LAB_POINT_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

CORE_Bool LabPoint_SetID(CORE_Handle LabPointHandle, uint32 ID);
CORE_Bool LabPoint_SetConnectionTop(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle);
CORE_Bool LabPoint_SetConnectionRight(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle);
CORE_Bool LabPoint_SetConnectionBottom(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle);
CORE_Bool LabPoint_SetConnectionLeft(CORE_Handle LabPointHandle, CORE_Handle ConnectionLabPointHandle);
CORE_Bool LabPoint_SetIsExit(CORE_Handle LabPointHandle, CORE_Bool Value);
CORE_Bool LabPoint_SetIsSpawn(CORE_Handle LabPointHandle, CORE_Bool Value);

CORE_Bool LabPoint_GetID(CORE_Handle LabPointHandle, uint32 *ID);
CORE_Bool LabPoint_GetConnectionTop(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle);
CORE_Bool LabPoint_GetConnectionRight(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle);
CORE_Bool LabPoint_GetConnectionBottom(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle);
CORE_Bool LabPoint_GetConnectionLeft(CORE_Handle LabPointHandle, CORE_Handle *ConnectionLabPointHandle);
CORE_Bool LabPoint_GetIsExit(CORE_Handle LabPointHandle, CORE_Bool *Value);
CORE_Bool LabPoint_GetIsSpawn(CORE_Handle LabPointHandle, CORE_Bool *Value);

CORE_Bool LabPoint_Create(CORE_Handle *LabPointHandle);
CORE_Bool LabPoint_Delete(CORE_Handle *LabPointHandle);

/*****************************************************************************************************************************/

#endif