#ifndef _LAB_GENERATION_H_
#define _LAB_GENERATION_H_

#include "CCORE.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

void LabGeneration_Execute(LabPointsMap *generated_lab_points_map, 
                           uint32       **out_spawn_points, 
                           uint32       *out_spawn_points_size);

/*****************************************************************************************************************************/

#endif
