#ifndef __LAB_GENERATION_H__
#define __LAB_GENERATION_H__

#include "../CORE/CORE-types.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

void LabGeneration_Execute(LabPointsMap GeneratedLabPointsMap, uint32 **OUT_SpawnPoints, uint32 *OUT_SpawnPointsSize);

/*****************************************************************************************************************************/

#endif