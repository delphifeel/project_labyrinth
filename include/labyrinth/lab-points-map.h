#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

CORE_Bool LabPointsMap_ToJSON(CORE_Handle LabPointsMapHandle, char **JSON);
CORE_Bool LabPointsMap_ToRawData(CORE_Handle LabPointsMapHandle, uint8 **RawData, uint32 *RawDataSize);
CORE_Bool LabPointsMap_AddPoint(CORE_Handle LabPointsMapHandle, CORE_Handle LabPointHandle);
CORE_Bool LabPointsMap_GetPointByID(CORE_Handle LabPointsMapHandle, uint32 ID, CORE_Handle *LabPointHandle);
CORE_Bool LabPointsMap_GetSize(CORE_Handle LabPointsMapHandle, uint32 *Size);
CORE_Bool LabPointsMap_GetCapacity(CORE_Handle LabPointsMapHandle, uint32 *Capacity);

CORE_Bool LabPointsMap_Create(CORE_Handle *LabPointsMapHandle);
CORE_Bool LabPointsMap_Delete(CORE_Handle *LabPointsMapHandle);

/*****************************************************************************************************************************/

#endif