#include "../include/labyrinth/lab-generation.h"

static void INTERNAL_ValidateGeneratedMap(LabPointsMap generated_map)
{
	uint32 			generated_map_size;
	LabPointStruct 	point;


	LabPointsMap_GetSize(generated_map, &generated_map_size);
	for (uint32 i = 0; i < generated_map_size; i++)
	{
		LabPointsMap_GetPointByID(generated_map, i + 1, &point);


		// every point has AT LEAST ONE connection
		if (point.TopConnectionId != 0)
			continue;

		if (point.RightConnectionId != 0)
			continue;

		if (point.BottomConnectionId != 0)
			continue;

		if (point.LeftConnectionId != 0)
			continue;

		CORE_DebugAbort("[%ld] Need to have AT LEAST ONE connection\n", i + 1);
	}
}

void Test_GenerateLab()
{
	LabPointsMap 	generated_map;
	uint32  		*spawn_points;
	uint32  		spawn_points_size;
	

	LabPointsMap_Create(&generated_map);

	LabGeneration_Execute(generated_map, &spawn_points, &spawn_points_size);
	INTERNAL_ValidateGeneratedMap(generated_map);

	CORE_MemFree(spawn_points);
	LabPointsMap_Free(&generated_map);
}

int main()
{
	Test_GenerateLab();
}