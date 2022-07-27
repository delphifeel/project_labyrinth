#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "CCORE.h"
#include "modules/player/player.h"
#include "lab-point.h"


typedef struct LabPointsMap_s LabPointsMap;

/*****************************************************************************************************************************/

void          LabPointsMap_ToJSON(const LabPointsMap *lab_map, char **json);

bool          LabPointsMap_AssignPlayerToPoint(LabPointsMap *lab_map, Player *player, uint point_id);
bool          LabPointsMap_ChangePoint(LabPointsMap *lab_map, const LabPoint point);
bool          LabPointsMap_GetPointByID(const LabPointsMap *lab_map, uint id, LabPoint *point);
uint          LabPointsMap_GetSize(const LabPointsMap *lab_map);

LabPointsMap *LabPointsMap_Create(uint size);
void          LabPointsMap_Free(LabPointsMap *lab_map);

/*****************************************************************************************************************************/

#endif