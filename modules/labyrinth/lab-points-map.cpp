#include "libs/core/core.h"
#include "include/config.h"
#include "lab-points-map.h"

std::pair<LabPoint *, bool>
LabPointsMap::GetPointByID(uint id)
{
    CORE_Assert(m_points_map != std::nullopt);

    if (m_points_map->count(id) > 0) {
        return { &m_points_map->at(id), true };
    }
    return { nullptr, false };
}

