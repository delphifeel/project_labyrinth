#include "modules/labyrinth/lab-generation.h"

static void _ValidateGeneratedMap(const std::map<uint, LabPoint> &generated_map)
{
    for (const auto& [_key, point] : generated_map)
    {
        const PointConnections &connections = point.GetConnections();
        // every point has AT LEAST ONE connection
        if (connections.Top != 0)
            continue;

        if (connections.Right != 0)
            continue;

        if (connections.Bottom != 0)
            continue;

        if (connections.Left != 0)
            continue;

        CORE_Abort("[%u] Need to have AT LEAST ONE connection\n", point.GetId());
    }
}

static void _LabGenerationWorksProperly(void)
{
    std::map<uint, LabPoint>    generated_map;
    std::vector<uint>           spawn_points;
    

    lab::Generate(generated_map, spawn_points);
    _ValidateGeneratedMap(generated_map);
}

void LabGeneration_TestsRun(void)
{
    _LabGenerationWorksProperly();
}