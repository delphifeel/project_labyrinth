#ifndef _LAB_GENERATION_H_
#define _LAB_GENERATION_H_

#include <vector>
#include <map>
#include "core/core.h"
#include "lab-point.h"

/*****************************************************************************************************************************/

namespace lab {
    void Generate(std::map<uint, LabPoint> &points_map, 
                  std::vector<uint>        &spawn_points);
}


/*****************************************************************************************************************************/

#endif