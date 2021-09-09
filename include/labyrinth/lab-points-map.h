#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

typedef struct LabPointStruct {
	uint32			Id; 
	uint32 			TopConnectionId; 
	uint32 			RightConnectionId; 
	uint32 			BottomConnectionId; 
	uint32 			LeftConnectionId; 
	CORE_Bool 		IsExit; 
	CORE_Bool 		IsSpawn;
} LabPointStruct;

CORE_OBJECT_DEFINE(LabPointsMap);

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap, char **JSON);
void LabPointsMap_ToRawData(LabPointsMap, uint8 **RawData, uint32 *RawDataSize);
void LabPointsMap_AddPoint(LabPointsMap, LabPointStruct Point);
void LabPointsMap_ChangePoint(LabPointsMap, LabPointStruct Point);
void LabPointsMap_GetPointByID(LabPointsMap, uint32 ID, LabPointStruct *OUT_Point);
void LabPointsMap_GetSize(LabPointsMap, uint32 *Size);
void LabPointsMap_GetCapacity(LabPointsMap, uint32 *Capacity);

void LabPointsMap_Create(LabPointsMap*);
void LabPointsMap_Free(LabPointsMap*);

/*****************************************************************************************************************************/

#endif
