#include "gameserver/lab-points-map-reader.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(LabPointsMapReader,
	LabPointsMap source;
);

/*****************************************************************************************************************************/

void LabPointsMapReader_GetPointByID(LabPointsMapReader instance, uint32 id, LabPointStruct *out_point)
{
	LabPointsMap_GetPointByID(instance->source, id, out_point);
}

void LabPointsMapReader_Setup(LabPointsMapReader instance, LabPointsMap source)
{
	instance->source = source;
}

/*****************************************************************************************************************************/

void LabPointsMapReader_Create(LabPointsMapReader* instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, LabPointsMapReader);
}

void LabPointsMapReader_Free(LabPointsMapReader* instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
