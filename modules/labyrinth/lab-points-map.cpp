#include "libs/core/core.h"
#include "include/config.h"
#include "lab-points-map.h"
#include "lab-generation.h"

LabPoint* LabPointsMap::GetPointByID(uint id)
{
    if (m_points_map.count(id) > 0) {
        return &m_points_map.at(id);
    }
    return nullptr;
}

void LabPointsMap::Generate(std::vector<uint> &spawn_points)
{
    lab::Generate(m_points_map, spawn_points);
}
