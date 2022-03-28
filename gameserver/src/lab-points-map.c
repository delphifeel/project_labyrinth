#include <string.h>

#include "CCORE.h"
#include "gameserver/lab-points-map.h"

/*****************************************************************************************************************************/

#define LABPOINTSMAP_DEFAULTCAPACITY 	(40)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(LabPointsMap,
	LabPointStruct 	*points_hash_map;
	uint32 			size;
	uint32 			capacity;
);

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(LabPointsMap instance, char **json)
{
	#define RAW_JSON_OBJECT_MAX_SIZE 	(120)
	#define DEC_JSON_LEFT(SIZE_TO_DEC) (json_size_left = ((SIZE_TO_DEC) > json_size_left ? 0 : (json_size_left - (SIZE_TO_DEC)))) 

	int32 				json_size_left;
	uint32 				max_json;
	LabPointStruct		current_lab_point; 
	char 				lab_point_raw_json_object[RAW_JSON_OBJECT_MAX_SIZE];
	uint32 				lab_point_raw_json_object_char_count;


	#ifdef CORE_DEBUGENABLED
	LabPointsMap_InternalPrint(instance);
	#endif

	max_json = instance->size * RAW_JSON_OBJECT_MAX_SIZE + 40;
	json_size_left = max_json;
	*json = CORE_MemAlloc(sizeof(char), max_json + 1);
	*json[0] = '\0';

	lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "{\"count\": %u,\"points\": {", instance->size);
	strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
	DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

	for (uint32 i = 0; i < instance->size; i++)
	{
		
		current_lab_point = instance->points_hash_map[i]; 
		lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "\"%u\": ", current_lab_point.Id);
		strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
		DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

		lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, 
												  "{\"top_id\": %u, \"right_id\": %u, \"bottom_id\": %u, \"left_id\": %u, \"is_exit\": %u, \"is_spawn\": %u}",
												  current_lab_point.top_connection_id, current_lab_point.right_connection_id, 
												  current_lab_point.bottom_connection_id, current_lab_point.left_connection_id,
												  current_lab_point.is_exit, current_lab_point.is_spawn);

		strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
		DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

		if (i != instance->size - 1)
		{
			strncat(*json, ",", 1);
			DEC_JSON_LEFT(1);
		}
	}

	strncat(*json, "}}", 2);
	DEC_JSON_LEFT(2);
}

void LabPointsMap_AddPoint(LabPointsMap instance, LabPointStruct point)
{

	if (instance->size == instance->capacity)		// if true, double capacity. 
	{
		instance->capacity += instance->capacity;
		instance->points_hash_map = CORE_MemRealloc(instance->points_hash_map, sizeof(LabPointStruct), instance->capacity);
		if (instance->points_hash_map == NULL)
		{
			CORE_DebugError("instance->points_hash_map == NULL\n");
			return;
		}
	}

	instance->points_hash_map[point.Id - 1] = point;
	instance->size++;
}

void LabPointsMap_ChangePoint(LabPointsMap instance, LabPointStruct point)
{
	instance->points_hash_map[point.Id - 1] = point;
}

void LabPointsMap_GetPointByID(LabPointsMap instance, uint32 id, LabPointStruct *out_point)
{
	if (id == 0)
	{
		CORE_DebugError("id cannot be 0\n");
		return;
	}

	*out_point = instance->points_hash_map[id - 1];
}

void LabPointsMap_GetSize(LabPointsMap instance, uint32 *size)
{
	*size = instance->size;
}

void LabPointsMap_GetCapacity(LabPointsMap instance, uint32 *capacity)
{
	*capacity = instance->capacity;
}

/*****************************************************************************************************************************/

void LabPointsMap_Create(LabPointsMap *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, LabPointsMap);

	(*instance_ptr)->capacity = LABPOINTSMAP_DEFAULTCAPACITY;
	(*instance_ptr)->size = 0;
	(*instance_ptr)->points_hash_map = CORE_MemAlloc(sizeof(LabPointStruct), (*instance_ptr)->capacity);
}

void LabPointsMap_Free(LabPointsMap *instance_ptr)
{
	CORE_MemFree((*instance_ptr)->points_hash_map);
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
