#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

typedef struct LabPointStruct {
	uint32			Id; 
	uint32 			top_connection_id; 
	uint32 			right_connection_id; 
	uint32 			bottom_connection_id; 
	uint32 			left_connection_id; 
	CORE_Bool 		is_exit; 
	CORE_Bool 		is_spawn;
} LabPointStruct;

CORE_OBJECT_DEFINE(LabPointsMap);

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap, char **json);
void LabPointsMap_ToRawData(LabPointsMap, uint8 **raw_data, uint32 *raw_data_size);
void LabPointsMap_AddPoint(LabPointsMap, LabPointStruct point);
void LabPointsMap_ChangePoint(LabPointsMap, LabPointStruct point);
void LabPointsMap_GetPointByID(LabPointsMap, uint32 id, LabPointStruct *out_point);
void LabPointsMap_GetSize(LabPointsMap, uint32 *size);
void LabPointsMap_GetCapacity(LabPointsMap, uint32 *capacity);

void LabPointsMap_Create(LabPointsMap*);
void LabPointsMap_Free(LabPointsMap*);

/*****************************************************************************************************************************/

#endif
