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
    explicit LabPointsMap() {};
    explicit LabPointsMap(LabPointsMap&) = delete;

    void                            ToJSON(char **json)                 const;
    LabPoint*                       GetPointByID(uint id);
    void                            Generate(std::vector<uint> &spawn_points);

private:
    std::map<uint, LabPoint>    m_points_map;
};


#endif