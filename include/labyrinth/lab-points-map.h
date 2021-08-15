#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "../CORE.h"

#include "lab-point.h"

CORE_OBJECT_INTERFACE(LabPointsMap,
	LabPoint 	*LabPointsArray;
	uint32 		Size;
	uint32 		Capacity;
)

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap, char **JSON);
void LabPointsMap_ToRawData(LabPointsMap, uint8 **RawData, uint32 *RawDataSize);
void LabPointsMap_AddPoint(LabPointsMap, LabPoint Point);
void LabPointsMap_GetPointByID(LabPointsMap, uint32 ID, LabPoint *OUT_Point);
void LabPointsMap_GetSize(LabPointsMap, uint32 *Size);
void LabPointsMap_GetCapacity(LabPointsMap, uint32 *Capacity);

void LabPointsMap_Create(LabPointsMap*);
void LabPointsMap_Free(LabPointsMap*);

/*****************************************************************************************************************************/

#endif