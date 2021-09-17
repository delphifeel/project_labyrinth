#include <stdlib.h>
#include <time.h>

#include "../include/labyrinth/lab-generation.h"
#include "../include/CORE.h"
#include "../include/labyrinth.h"
#include "../include/disjoint-set.h"

/*****************************************************************************************************************************/

#define MATRIX_SIZE 		(41)
#define SPAWN_POINTS_COUNT 	(20)

typedef struct Edge
{
	uint32 from;
	uint32 to;
} Edge;

static void INTERNAL_FillRectangleLab(LabPointsMap temp_points_map, uint32 *spawn_points)
{
	LabPointStruct lab_point;
	int32 connection_id, id, p;


	if (MATRIX_SIZE < 7) {
		CORE_DebugError("MATRIX_SIZE need to be >= 7\n");
	}

	if (MATRIX_SIZE % 2 == 0) {
		CORE_DebugError("MATRIX_SIZE need to be odd\n");
	}


	// create all points
	p = MATRIX_SIZE * MATRIX_SIZE;
	for (uint32 i = 0; i < p; i++)
	{
		lab_point.Id = i + 1;
		
		lab_point.top_connection_id = 0;
		lab_point.right_connection_id = 0;
		lab_point.left_connection_id = 0;
		lab_point.bottom_connection_id = 0;
		lab_point.is_exit = FALSE;
		lab_point.is_spawn = FALSE;

		LabPointsMap_AddPoint(temp_points_map, lab_point);
	}


	// set exit point
	LabPointsMap_GetPointByID(temp_points_map, p / 2 + 1, &lab_point);
	CORE_DebugPrint("Set %ld as exit\n", p / 2 + 1);
	lab_point.is_exit = TRUE;
	LabPointsMap_ChangePoint(temp_points_map, lab_point);


	// set spawn points (center square of matrix)
	uint32 possible_spawn_points[p / 2];
	uint32 possible_spawn_points_size = 0;
	uint32 row_index, column_index, pos_index, added_points;

	pos_index = MATRIX_SIZE / 4;

	row_index = pos_index;
	for (column_index = pos_index + 2; column_index < MATRIX_SIZE - pos_index + 1; column_index++)
	{
		possible_spawn_points[possible_spawn_points_size] = row_index * MATRIX_SIZE + column_index;
		possible_spawn_points_size++;
	}

	column_index = MATRIX_SIZE - pos_index;
	for (row_index = pos_index + 1; row_index < MATRIX_SIZE - pos_index; row_index++)
	{
		possible_spawn_points[possible_spawn_points_size] = row_index * MATRIX_SIZE + column_index;
		possible_spawn_points_size++;
	}

	row_index = MATRIX_SIZE - pos_index - 1;
	for (column_index = MATRIX_SIZE - pos_index - 1; column_index > pos_index; column_index--)
	{
		possible_spawn_points[possible_spawn_points_size] = row_index * MATRIX_SIZE + column_index;
		possible_spawn_points_size++;
	}

	column_index = pos_index + 1;
	for (row_index = MATRIX_SIZE - pos_index - 2; row_index > pos_index - 1; row_index--)
	{
		possible_spawn_points[possible_spawn_points_size] = row_index * MATRIX_SIZE + column_index;
		possible_spawn_points_size++;
	}

	added_points = 0;
	for (uint32 i = 0; i < possible_spawn_points_size; i++)
	{
		if (i % (possible_spawn_points_size / SPAWN_POINTS_COUNT) != 0) {
			continue;
		}

		LabPointsMap_GetPointByID(temp_points_map, possible_spawn_points[i], &lab_point);

		lab_point.is_spawn = TRUE; 
		LabPointsMap_ChangePoint(temp_points_map, lab_point);
		spawn_points[added_points] = possible_spawn_points[i];
		added_points++;

		if (added_points == SPAWN_POINTS_COUNT) {
			break;
		}
	}


	// fill connections for every point
	for (uint32 i = 0; i < p; i++)
	{
		id = i + 1;
		LabPointsMap_GetPointByID(temp_points_map, id, &lab_point);

		// top connection
		connection_id = id - MATRIX_SIZE;
		if (connection_id > 0) {
			lab_point.top_connection_id = connection_id;
		}

		// right connection
		connection_id = id + 1;
		if (connection_id % MATRIX_SIZE != 1) {
			lab_point.right_connection_id = connection_id;
		}

		// bottom connection
		connection_id = id + MATRIX_SIZE;
		if (connection_id <= p) {
			lab_point.bottom_connection_id = connection_id;
		}
		
		// left connection
		connection_id = id - 1;
		if (connection_id % MATRIX_SIZE != 0) {
			lab_point.left_connection_id = connection_id; 
		}

		LabPointsMap_ChangePoint(temp_points_map, lab_point);
	}
}

static int INTERNAL_SortEdgesRandomly(const void *unused1, const void *unused2)
{
	int32 random_number;


	random_number = rand() % 3;
	switch (random_number)
	{
		case 0: return -1;
		case 1: return 0;
		case 2: return 1;
	}

	return 0;
}

static void INTERNAL_CopyConnectionsAccordingToEdge(LabPointsMap temp_points_map, LabPointsMap result_lab_points_map_handle, uint32 lab_point_id, uint32 connection_lab_point_id)
{
	LabPointStruct source_point, result_point;
	

	LabPointsMap_GetPointByID(temp_points_map, lab_point_id, &source_point);
	LabPointsMap_GetPointByID(result_lab_points_map_handle, lab_point_id, &result_point);

	if (source_point.top_connection_id == connection_lab_point_id)
	{
		result_point.top_connection_id = connection_lab_point_id;
	}
	else if (source_point.right_connection_id == connection_lab_point_id)
	{
		result_point.right_connection_id = connection_lab_point_id;
	}
	else if (source_point.bottom_connection_id == connection_lab_point_id)
	{
		result_point.bottom_connection_id = connection_lab_point_id;
	}
	else if (source_point.left_connection_id == connection_lab_point_id)
	{
		result_point.left_connection_id = connection_lab_point_id;
	}

	LabPointsMap_ChangePoint(result_lab_points_map_handle, result_point);
}

static void INTERNAL_BuildMSTMaze(LabPointsMap temp_points_map, LabPointsMap mst_points_map_handle)
{
	LabPointStruct 	current_lab_point_handle, temp_lab_point_handle, lab_point; 
	DisjointSet 	disjoint_set_handle;
	uint32 			vertex_count, id;
	Edge 			*sorted_edges, *mst_edges;
	uint32 			sorted_edges_size, mst_edges_size, subsets_left;


	sorted_edges = CORE_MemAlloc(sizeof(Edge) * MATRIX_SIZE * MATRIX_SIZE * 4);
	mst_edges = CORE_MemAlloc(sizeof(Edge) * MATRIX_SIZE * MATRIX_SIZE * 4);

	// get all possible edges
	sorted_edges_size = 0;
	LabPointsMap_GetSize(temp_points_map, &vertex_count);
	for (uint32 i = 0; i < vertex_count; i++)
	{
		id = i + 1;
		LabPointsMap_GetPointByID(temp_points_map, id, &current_lab_point_handle);

		if (current_lab_point_handle.top_connection_id != 0)
		{
			sorted_edges[sorted_edges_size].from = id; 
			sorted_edges[sorted_edges_size].to = current_lab_point_handle.top_connection_id;
			sorted_edges_size++;
		}
		
		if (current_lab_point_handle.right_connection_id != 0)
		{
			sorted_edges[sorted_edges_size].from = id; 
			sorted_edges[sorted_edges_size].to = current_lab_point_handle.right_connection_id;
			sorted_edges_size++;
		}

		if (current_lab_point_handle.bottom_connection_id != 0)
		{
			sorted_edges[sorted_edges_size].from = id; 
			sorted_edges[sorted_edges_size].to = current_lab_point_handle.bottom_connection_id;
			sorted_edges_size++;
		}

		if (current_lab_point_handle.left_connection_id != 0)
		{
			sorted_edges[sorted_edges_size].from = id; 
			sorted_edges[sorted_edges_size].to = current_lab_point_handle.left_connection_id;
			sorted_edges_size++;
		}
	}


	// sort edges randomly
	srand(time(NULL));
	qsort(sorted_edges, sorted_edges_size, sizeof(Edge), INTERNAL_SortEdgesRandomly);

	CORE_DebugPrint("sorted_edges_size: %ld\n", sorted_edges_size);


	// create disjoint set from edges and union all possible edges according to Kruskal's algo
	DisjointSet_Create(&disjoint_set_handle);
	DisjointSet_Setup(disjoint_set_handle, vertex_count + 1);
	mst_edges_size = 0;
	
	for (uint32 i = 0; i < sorted_edges_size; i++)
	{
		if (DisjointSet_Union(disjoint_set_handle, sorted_edges[i].from, sorted_edges[i].to) == FALSE) {
			continue;
		}

		mst_edges[mst_edges_size].from = sorted_edges[i].from;
		mst_edges[mst_edges_size].to = sorted_edges[i].to;
		mst_edges_size++;
	}

	DisjointSet_GetSubsetsCount(disjoint_set_handle, &subsets_left);
	DisjointSet_Free(&disjoint_set_handle);

	CORE_DebugPrint("subsets_left: %ld\n", subsets_left);
	CORE_DebugPrint("mst_edges_size: %ld\n", mst_edges_size);


	// create new lab points map accordoing to mst_edges
	for (uint32 i = 0; i < vertex_count; i++)
	{
		LabPointsMap_GetPointByID(temp_points_map, i + 1, &temp_lab_point_handle);

		lab_point.Id = i + 1; 
		lab_point.top_connection_id = 0;
		lab_point.right_connection_id = 0;
		lab_point.left_connection_id = 0;
		lab_point.bottom_connection_id = 0;
		lab_point.is_exit = temp_lab_point_handle.is_exit;
		lab_point.is_spawn = temp_lab_point_handle.is_spawn;

		LabPointsMap_AddPoint(mst_points_map_handle, lab_point);
	}

	for (uint32 i = 0; i < mst_edges_size; i++)
	{
		if ((mst_edges[i].from == 0) || (mst_edges[i].to == 0)) {
			continue;
		}

		INTERNAL_CopyConnectionsAccordingToEdge(temp_points_map, mst_points_map_handle, mst_edges[i].from, mst_edges[i].to);
		INTERNAL_CopyConnectionsAccordingToEdge(temp_points_map, mst_points_map_handle, mst_edges[i].to, mst_edges[i].from);
	}

	CORE_MemFree(sorted_edges);
	CORE_MemFree(mst_edges);
}

/*****************************************************************************************************************************/

void LabGeneration_Execute(LabPointsMap generated_lab_points_map, uint32 **out_spawn_points, uint32 *out_spawn_points_size)
{
	LabPointsMap 		temp_points_map;


	LabPointsMap_Create(&temp_points_map);

	*out_spawn_points_size = SPAWN_POINTS_COUNT;
	*out_spawn_points = CORE_MemAlloc(sizeof(uint32) * *out_spawn_points_size);
	INTERNAL_FillRectangleLab(temp_points_map, *out_spawn_points);

	INTERNAL_BuildMSTMaze(temp_points_map, generated_lab_points_map);

	LabPointsMap_Free(&temp_points_map);
}

/*****************************************************************************************************************************/
