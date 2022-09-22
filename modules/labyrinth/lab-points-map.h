#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include <utility>
#include <optional>
#include <vector>
#include <map>

#include "libs/core/core.h"

#include "lab-point.h"

class LabPointsMap
{
public:
    void                            ToJSON(char **json)                 const;
    std::pair<LabPoint *, bool>     GetPointByID(uint id);
    void                            SetMap(const std::map<uint, LabPoint> &points_map) { m_points_map = points_map; };

    explicit LabPointsMap() :
        m_points_map(std::nullopt) {};

private:
    std::optional< std::map<uint, LabPoint> >    m_points_map;
};


#endif