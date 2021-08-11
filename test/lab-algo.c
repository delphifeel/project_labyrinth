#include "../include/CORE.h"
#include "../include/labyrinth.h"

#define MAX_LEVEL 	(8)

enum {
	PARENTPOINT_NONE,
	PARENTPOINT_TOP,
	PARENTPOINT_RIGHT,
	PARENTPOINT_BOTTOM,
	PARENTPOINT_LEFT,
};

void buildFromPoint(CORE_Handle LabPointsMapHandle, uint32 *NextIndex, CORE_Handle ParentPointHandle, uint32 ParentPointDirection, uint32 Level)
{
	CORE_Handle CreatedLabPoint;

	LabPoint_Create(&CreatedLabPoint);
	LabPoint_SetID(CreatedLabPoint, *NextIndex + 1);
	(*NextIndex)++;

	switch (ParentPointDirection)
	{
		case PARENTPOINT_TOP:
			{
			LabPoint_SetConnectionTop(CreatedLabPoint, ParentPointHandle);
			LabPoint_SetConnectionBottom(ParentPointHandle, CreatedLabPoint);
			break;
			}

		case PARENTPOINT_RIGHT:
			{
			LabPoint_SetConnectionRight(CreatedLabPoint, ParentPointHandle); 
			LabPoint_SetConnectionLeft(ParentPointHandle, CreatedLabPoint); 
			break;
			}

		case PARENTPOINT_BOTTOM: 	
			{
			LabPoint_SetConnectionBottom(CreatedLabPoint, ParentPointHandle); 
			LabPoint_SetConnectionTop(ParentPointHandle, CreatedLabPoint); 
			break;
			}

		case PARENTPOINT_LEFT:
			{
			LabPoint_SetConnectionLeft(CreatedLabPoint, ParentPointHandle);
			LabPoint_SetConnectionRight(ParentPointHandle, CreatedLabPoint);
			break;
			}
	}

	LabPointsMap_AddPoint(LabPointsMapHandle, CreatedLabPoint);

	if (Level == MAX_LEVEL)
		return;

	if (ParentPointDirection != PARENTPOINT_TOP)
		buildFromPoint(LabPointsMapHandle, NextIndex, CreatedLabPoint, PARENTPOINT_BOTTOM, Level + 1);
	if (ParentPointDirection != PARENTPOINT_RIGHT)
		buildFromPoint(LabPointsMapHandle, NextIndex, CreatedLabPoint, PARENTPOINT_LEFT, Level + 1);
	if (ParentPointDirection != PARENTPOINT_BOTTOM)
		buildFromPoint(LabPointsMapHandle, NextIndex, CreatedLabPoint, PARENTPOINT_TOP, Level + 1);
	if (ParentPointDirection != PARENTPOINT_LEFT)
		buildFromPoint(LabPointsMapHandle, NextIndex, CreatedLabPoint, PARENTPOINT_RIGHT, Level + 1);
}

void buildLabyrinth()
{
	CORE_Handle 		LabPointsMapHandle, EntryLabPoint;
	CORE_FileHandle 	FileToWrite;
	char 				*JSON;
	uint32 				NextIndex;


	NextIndex = 0;

	LabPointsMap_Create(&LabPointsMapHandle);
	buildFromPoint(LabPointsMapHandle, &NextIndex, NULL, PARENTPOINT_NONE, 1);
	LabPointsMap_ToJSON(LabPointsMapHandle, &JSON);

	FileToWrite = CORE_FileOpen("data.json", "w");
	CORE_FileWrite(FileToWrite, sizeof(char), CORE_StringLength(JSON), JSON);
	CORE_FileClose(FileToWrite);

	CORE_MemFree(JSON);
	LabPointsMap_Delete(&LabPointsMapHandle);
}

int main() 
{
	buildLabyrinth();
}