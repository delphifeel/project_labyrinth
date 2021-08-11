#include "../include/CORE.h"
#include "../include/labyrinth.h"


int main()
{
	CORE_Handle 		LabPointsMapHandle;
	CORE_Handle 		CurrentLabPoint;
	CORE_FileHandle 	FileToWrite;
	uint32 				Size, Capacity;
	char 				*JSON;

	LabPointsMap_Create(&LabPointsMapHandle);

	for (int i = 0; i < 10; i++)
		{
		LabPoint_Create(&CurrentLabPoint);
		LabPoint_SetID(CurrentLabPoint, i + 1);
		LabPointsMap_AddPoint(LabPointsMapHandle, CurrentLabPoint);
		}

	LabPointsMap_GetSize(LabPointsMapHandle, &Size);
	LabPointsMap_GetCapacity(LabPointsMapHandle, &Capacity);

	CORE_DebugPrint("Size: %ld\nCapacity: %ld\n", Size, Capacity);

	LabPointsMap_ToJSON(LabPointsMapHandle, &JSON);

	CORE_MemFree(JSON);
	LabPointsMap_Delete(&LabPointsMapHandle);
}