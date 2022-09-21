#include "core/core.h"
#include "config.h"
#include "modules/labyrinth/lab-points-map.h"

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

