#include "modules/labyrinth/lab-generation.h"

static void _ValidateGeneratedMap(LabPointsMap *generated_map)
{
    LabPoint        point;


    uint generated_map_size = LabPointsMap_GetSize(generated_map);
    for (uint32 i = 0; i < generated_map_size; i++)
    {
        LabPointsMap_GetPointByID(generated_map, i + 1, &point);


        // every point has AT LEAST ONE connection
        if (point.top_connection_id != 0)
            continue;

        if (point.right_connection_id != 0)
            continue;

        if (point.bottom_connection_id != 0)
            continue;

        if (point.left_connection_id != 0)
            continue;

        CORE_Abort("[%u] Need to have AT LEAST ONE connection\n", i + 1);
    }
}

static void _LabGenerationWorksProperly(void)
{
    LabPointsMap    *generated_map;
    uint            *spawn_points;
    uint             spawn_points_size;
    

    LabGeneration_Execute(&generated_map, &spawn_points, &spawn_points_size);
    _ValidateGeneratedMap(generated_map);

    CORE_MemFree(spawn_points);
    LabPointsMap_Free(generated_map);
}

void LabGeneration_TestsRun(void)
{
    _LabGenerationWorksProperly();
}