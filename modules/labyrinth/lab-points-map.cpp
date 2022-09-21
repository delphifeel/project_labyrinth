#include "libs/core/core.h"
#include "include/config.h"
#include "lab-points-map.h"

std::optional<const LabPoint *> 
LabPointsMap::GetPointByID(uint id) const
{
    if (m_points_map.count(id) > 0) {
        return &m_points_map.at(id);
    }
    return std::nullopt;
}

void 
LabPointsMap::ChangePoint(const LabPoint &point)
{
    m_points_map[point.GetId()] = point;
}

