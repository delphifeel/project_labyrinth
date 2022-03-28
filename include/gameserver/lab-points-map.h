#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "CCORE.h"

#define LABPOINT_SIZE 	(40)

/*****************************************************************************************************************************/

typedef struct LabPointStruct {
	uint32			Id; 
	uint32 			top_connection_id; 
	uint32 			right_connection_id; 
	uint32 			bottom_connection_id; 
	uint32 			left_connection_id; 
	bool 		is_exit; 
	bool 		is_spawn;
} LabPointStruct;

CORE_OBJECT_DEFINE(LabPointsMap);

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap instance, char **json);
void LabPointsMap_ToRawData(LabPointsMap, uint8 **raw_data, uint32 *raw_data_size);
void LabPointsMap_AddPoint(LabPointsMap instance, LabPointStruct point);
void LabPointsMap_ChangePoint(LabPointsMap instance, LabPointStruct point);
void LabPointsMap_GetPointByID(LabPointsMap instance, uint32 id, LabPointStruct *out_point);
void LabPointsMap_GetSize(LabPointsMap instance, uint32 *size);
void LabPointsMap_GetCapacity(LabPointsMap instance, uint32 *capacity);

void LabPointsMap_Create(LabPointsMap* instance_ptr);
void LabPointsMap_Free(LabPointsMap* instance_ptr);

/*****************************************************************************************************************************/

#endif
