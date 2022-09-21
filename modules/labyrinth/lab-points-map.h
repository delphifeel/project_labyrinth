#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include <optional>
#include <vector>
#include <map>

#include "libs/core/core.h"
#include "lab-point.h"

class LabPointsMap
{
public:
    void                            ToJSON(char **json)                 const;
    std::optional<const LabPoint *> GetPointByID(uint id)               const;
    
    void                            ChangePoint(const LabPoint &point);

    explicit LabPointsMap() {};

private:
    std::map<uint, LabPoint>                        m_points_map;
};


#endif