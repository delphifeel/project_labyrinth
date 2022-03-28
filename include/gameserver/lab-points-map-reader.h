#ifndef _LAB_POINTS_MAP_READER_H_
#define _LAB_POINTS_MAP_READER_H_

#include "CCORE.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(LabPointsMapReader);

/*****************************************************************************************************************************/

void LabPointsMapReader_GetPointByID(LabPointsMapReader instance, uint32 id, LabPointStruct *out_point);

void LabPointsMapReader_Setup(LabPointsMapReader instance, LabPointsMap source);

void LabPointsMapReader_Create(LabPointsMapReader* instance_ptr);
void LabPointsMapReader_Free(LabPointsMapReader* instance_ptr);

/*****************************************************************************************************************************/

#endif
