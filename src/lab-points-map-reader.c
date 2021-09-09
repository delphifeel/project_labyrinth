#include "../include/labyrinth/lab-points-map-reader.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(LabPointsMapReader,
	LabPointsMap Source;
);

/*****************************************************************************************************************************/

void LabPointsMapReader_GetPointByID(LabPointsMapReader Instance, uint32 ID, LabPointStruct *OUT_Point)
{
	LabPointsMap_GetPointByID(Instance->Source, ID, OUT_Point);
}

void LabPointsMapReader_Setup(LabPointsMapReader Instance, LabPointsMap Source)
{
	Instance->Source = Source;
}

/*****************************************************************************************************************************/

void LabPointsMapReader_Create(LabPointsMapReader* InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, LabPointsMapReader);
}

void LabPointsMapReader_Free(LabPointsMapReader* InstancePtr)
{
	CORE_OBJECT_FREE(InstancePtr);
}

/*****************************************************************************************************************************/
