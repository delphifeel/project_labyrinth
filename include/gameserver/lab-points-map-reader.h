#ifndef __LAB_POINTS_MAP_READER_H__
#define __LAB_POINTS_MAP_READER_H__

#include "CORE.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(LabPointsMapReader);

/*****************************************************************************************************************************/

void LabPointsMapReader_GetPointByID(LabPointsMapReader, uint32 id, LabPointStruct *out_point);

void LabPointsMapReader_Setup(LabPointsMapReader, LabPointsMap source);

void LabPointsMapReader_Create(LabPointsMapReader*);
void LabPointsMapReader_Free(LabPointsMapReader*);

/*****************************************************************************************************************************/

#endif
