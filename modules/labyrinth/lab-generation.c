#include <stdlib.h>
#include <time.h>

#include "CCORE.h"
#include "modules/labyrinth/lab-generation.h"

/*****************************************************************************************************************************/

#define MIN_MATRIX_SIZE     (7)

#define MATRIX_SIZE         (13)
#define SPAWN_POINTS_COUNT  (4)

typedef struct Edge
{
    uint from;
    uint to;
} Edge;

// build rectengular graph where vertexes are points(rooms) and edges are 
// connections among them
static void _BuildLab(LabPointsMap *fully_connected_map, uint *spawn_points)
{
    LabPoint      lab_point;
    int           connection_id;
    int           id;
    int           p;


    CORE_AssertWithMessage(MATRIX_SIZE >= MIN_MATRIX_SIZE, "MATRIX_SIZE need to be >= 7\n");
    CORE_AssertWithMessage(MATRIX_SIZE % 2 != 0, "MATRIX_SIZE need to be odd\n");

    // create all points(rooms)
    p = MATRIX_SIZE * MATRIX_SIZE;
    for (uint i = 0; i < p; i++)
    {
        lab_point.Id = i + 1;
        
        lab_point.top_connection_id = 0;
        lab_point.right_connection_id = 0;
        lab_point.left_connection_id = 0;
        lab_point.bottom_connection_id = 0;
        lab_point.is_exit = false;
        lab_point.is_spawn = false;

        LabPointsMap_ChangePoint(fully_connected_map, lab_point);
    }


    // set exit point
    LabPointsMap_GetPointByID(fully_connected_map, p / 2 + 1, &lab_point);
    CORE_DebugInfo("Set %u as exit\n", p / 2 + 1);
    lab_point.is_exit = true;
    LabPointsMap_ChangePoint(fully_connected_map, lab_point);


    // set spawn points (center square of matrix)
    uint possible_spawn_points[p / 2];
    uint possible_spawn_points_size = 0;
    uint row_index;
    uint column_index;
    uint pos_index;
    uint added_points;

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
    for (uint i = 0; i < possible_spawn_points_size; i++)
    {
        if (i % (possible_spawn_points_size / SPAWN_POINTS_COUNT) != 0) {
            continue;
        }

        LabPointsMap_GetPointByID(fully_connected_map, possible_spawn_points[i], &lab_point);

        lab_point.is_spawn = true; 
        LabPointsMap_ChangePoint(fully_connected_map, lab_point);
        spawn_points[added_points] = possible_spawn_points[i];
        added_points++;

        if (added_points == SPAWN_POINTS_COUNT) {
            break;
        }
    }


    /**
     * connect each point(room) with top+right+bottom+left 
     * to build a rectengular net
     */
    for (uint i = 0; i < p; i++)
    {
        id = i + 1;
        LabPointsMap_GetPointByID(fully_connected_map, id, &lab_point);

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

        LabPointsMap_ChangePoint(fully_connected_map, lab_point);
    }
}

#define RANDOM_DATA_SIZE    (100)

static uint8  random_data[RANDOM_DATA_SIZE];
static uint   random_data_pos = 0;

static int _SortEdgesRandomly(const void *unused1, const void *unused2)
{
    int random_number;


    random_number = random_data[random_data_pos++] % 3;
    if (random_data_pos == RANDOM_DATA_SIZE)
    {
        random_data_pos = 0;
    }

    switch (random_number)
    {
        case 0:     return -1;
        case 1:     return 0;
        case 2:     return 1;
        default:    return 0;
    }

    return 0;
}

static void _CopyConnectionsAccordingToEdge(LabPointsMap *fully_connected_map, 
                                            LabPointsMap *result_lab_points_map_handle, 
                                            uint          lab_point_id, 
                                            uint          connection_lab_point_id)
{
    LabPoint source_point;
    LabPoint result_point;
    

    LabPointsMap_GetPointByID(fully_connected_map, lab_point_id, &source_point);
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

/**
 * @brief      Build MST from fully connected map randomly selecting edges
 *
 * @param[in]  fully_connected_map    Fully connected map (rectengular net)
 * @param[in]  mst_points_map_handle  result MST map
 */
static void _BuildMSTFrom(LabPointsMap *fully_connected_map, LabPointsMap *mst_points_map_handle)
{
    LabPoint        current_lab_point_handle;
    LabPoint        temp_lab_point_handle;
    LabPoint        lab_point; 
    CDisjointSet   *disjoint_set_handle;
    uint            vertex_count;
    uint            id;
    Edge           *sorted_edges;
    Edge           *mst_edges;
    uint            sorted_edges_size;
    uint            mst_edges_size;
    uint            subsets_count;


    sorted_edges = CORE_MemAlloc(sizeof(Edge), MATRIX_SIZE * MATRIX_SIZE * 4);
    mst_edges = CORE_MemAlloc(sizeof(Edge), MATRIX_SIZE * MATRIX_SIZE * 4);

    // copy all edges to sorted_edges array ...
    sorted_edges_size = 0;
    vertex_count = LabPointsMap_GetSize(fully_connected_map);
    for (uint i = 0; i < vertex_count; i++)
    {
        id = i + 1;
        LabPointsMap_GetPointByID(fully_connected_map, id, &current_lab_point_handle);

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


    // ...and sort edges randomly
    CORE_GenerateRandomDataToBuffer(random_data, RANDOM_DATA_SIZE);
    qsort(sorted_edges, sorted_edges_size, sizeof(Edge), _SortEdgesRandomly);
    CORE_DebugInfo("Sorted edges count: %u\n", sorted_edges_size);


    // create disjoint set from edges and 
    // union all edges according to Kruskal's algo
    disjoint_set_handle = CDisjointSet_Create(vertex_count + 1);
    mst_edges_size = 0;
    
    for (uint i = 0; i < sorted_edges_size; i++) {
        if (!CDisjointSet_Union(disjoint_set_handle, sorted_edges[i].from, sorted_edges[i].to)) {
            continue;
        }

        mst_edges[mst_edges_size].from = sorted_edges[i].from;
        mst_edges[mst_edges_size].to = sorted_edges[i].to;
        mst_edges_size++;
    }

    CDisjointSet_GetSubsetsCount(disjoint_set_handle, &subsets_count);
    CDisjointSet_Free(&disjoint_set_handle);

    CORE_DebugInfo("Subsets after union: %u\n", subsets_count);
    CORE_DebugInfo("MST edges count: %u\n", mst_edges_size);


    // build new lab points map accordoing to mst_edges
    for (uint i = 0; i < vertex_count; i++)
    {
        LabPointsMap_GetPointByID(fully_connected_map, i + 1, &temp_lab_point_handle);

        lab_point.Id = i + 1; 
        lab_point.top_connection_id = 0;
        lab_point.right_connection_id = 0;
        lab_point.left_connection_id = 0;
        lab_point.bottom_connection_id = 0;
        lab_point.is_exit = temp_lab_point_handle.is_exit;
        lab_point.is_spawn = temp_lab_point_handle.is_spawn;

        LabPointsMap_ChangePoint(mst_points_map_handle, lab_point);
    }

    for (uint i = 0; i < mst_edges_size; i++)
    {
        if ((mst_edges[i].from == 0) || (mst_edges[i].to == 0)) {
            continue;
        }

        _CopyConnectionsAccordingToEdge(fully_connected_map, mst_points_map_handle, mst_edges[i].from, mst_edges[i].to);
        _CopyConnectionsAccordingToEdge(fully_connected_map, mst_points_map_handle, mst_edges[i].to, mst_edges[i].from);
    }

    CORE_MemFree(sorted_edges);
    CORE_MemFree(mst_edges);
}

/*****************************************************************************************************************************/

static void _SaveToFile(LabPointsMap *lab_map)
{
    char *json;
    CFile FileToWrite;

    LabPointsMap_ToJSON(lab_map, &json);

    FileToWrite = CFile_Open("LAB_ROOMS.json", "w");
    CFile_Write(json, sizeof(char), strlen(json), FileToWrite);
    CFile_Close(FileToWrite);

    CORE_MemFree(json);
}

void LabGeneration_Execute(LabPointsMap **generated_lab_points_map, 
                           uint         **out_spawn_points, 
                           uint          *out_spawn_points_size)
{   
    const uint          lab_size = MATRIX_SIZE * MATRIX_SIZE;


    LabPointsMap *fully_connected_map   = LabPointsMap_Create(lab_size);
    *generated_lab_points_map           = LabPointsMap_Create(lab_size);

    *out_spawn_points_size = SPAWN_POINTS_COUNT;
    *out_spawn_points = CORE_MemAlloc(sizeof(uint), *out_spawn_points_size);
    _BuildLab(fully_connected_map, *out_spawn_points);

    _BuildMSTFrom(fully_connected_map, *generated_lab_points_map);
    _SaveToFile(*generated_lab_points_map);

    LabPointsMap_Free(fully_connected_map);
}

/*****************************************************************************************************************************/