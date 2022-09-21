#include <cstdlib>
#include <time.h>
#include <cstdio>

#include "libs/core/core-dset.h"
#include "lab-generation.h"

/*****************************************************************************************************************************/

#define MIN_MATRIX_SIZE     (7)

#define MATRIX_SIZE         (63)
#define SPAWN_POINTS_COUNT  (16)

struct Edge
{
    uint from;
    uint to;
};

// build rectengular graph where vertexes are points(rooms) and edges are 
// connections among them
static void _BuildLab(std::map<uint, LabPoint> &fully_connected_map, std::vector<uint> &spawn_points)
{


    CORE_AssertWithMessage(MATRIX_SIZE >= MIN_MATRIX_SIZE, "MATRIX_SIZE need to be >= 7\n");
    CORE_AssertWithMessage(MATRIX_SIZE % 2 != 0, "MATRIX_SIZE need to be odd\n");

    // create all points(rooms)
    constexpr uint p = MATRIX_SIZE * MATRIX_SIZE;
    for (uint i = 0; i < p; i++) {
        const PointConnections  connections = { 0, 0, 0, 0 };
        uint                    id = i + 1;
        LabPoint                lab_point(id, connections);

        fully_connected_map[id].Copy(lab_point);
    }


    // set exit point

    CORE_DebugInfo("Set %u as exit\n", p / 2 + 1);
    fully_connected_map[p / 2 + 1].SetAsExit(true);



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

        fully_connected_map[possible_spawn_points[i]].SetAsSpawn(true);

        spawn_points.push_back(possible_spawn_points[i]);
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
        uint id = i + 1;
        PointConnections connections = fully_connected_map[id].GetConnections();

        int connection_id;
        // top connection
        connection_id = id - MATRIX_SIZE;
        if (connection_id > 0) {
            connections.Top = connection_id;
        }

        // right connection
        connection_id = id + 1;
        if (connection_id % MATRIX_SIZE != 1) {
            connections.Right = connection_id;
        }

        // bottom connection
        connection_id = id + MATRIX_SIZE;
        if (connection_id <= p) {
            connections.Bottom = connection_id;
        }
        
        // left connection
        connection_id = id - 1;
        if (connection_id % MATRIX_SIZE != 0) {
            connections.Left = connection_id; 
        }

        fully_connected_map[id].SetConnections(connections);
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

static void _CopyConnectionsAccordingToEdge(std::map<uint, LabPoint>    &fully_connected_map, 
                                            std::map<uint, LabPoint>    &result_lab_points_map_handle, 
                                            uint                         lab_point_id, 
                                            uint                         connection_lab_point_id)
{
    PointConnections source_point_conn = fully_connected_map[lab_point_id].GetConnections();
    PointConnections result_point_conn = result_lab_points_map_handle[lab_point_id].GetConnections();

    if (source_point_conn.Top == connection_lab_point_id)
    {
        result_point_conn.Top = connection_lab_point_id;
    }
    else if (source_point_conn.Right == connection_lab_point_id)
    {
        result_point_conn.Right = connection_lab_point_id;
    }
    else if (source_point_conn.Bottom == connection_lab_point_id)
    {
        result_point_conn.Bottom = connection_lab_point_id;
    }
    else if (source_point_conn.Left == connection_lab_point_id)
    {
        result_point_conn.Left = connection_lab_point_id;
    }

    result_lab_points_map_handle[lab_point_id].SetConnections(result_point_conn);
}

static bool _GenerateRandomDataToBuffer(uint8 buffer[], uint32 buffer_size)
{
    FILE*   file;
    uint    read_size;


    file = fopen("/dev/urandom", "re");
    if (file == NULL) 
    {
        return false;
    }

    read_size = fread(buffer, sizeof(uint8), buffer_size, file);
    if (read_size != buffer_size)
    {
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

/**
 * @brief      Build MST from fully connected map randomly selecting edges
 *
 * @param[in]  fully_connected_map    Fully connected map (rectengular net)
 * @param[in]  mst_points_map_handle  result MST map
 */
static void _BuildMSTFrom(std::map<uint, LabPoint>  &fully_connected_map, 
                          std::map<uint, LabPoint>  &mst_points_map_handle)
{
    uint                vertex_count;
    Edge                *sorted_edges;
    Edge                *mst_edges;
    uint                sorted_edges_size;
    uint                mst_edges_size;


    sorted_edges    = new Edge[MATRIX_SIZE * MATRIX_SIZE * 4];
    mst_edges       = new Edge[MATRIX_SIZE * MATRIX_SIZE * 4];

    // copy all edges to sorted_edges array ...
    sorted_edges_size = 0;
    vertex_count = fully_connected_map.size();
    for (uint i = 0; i < vertex_count; i++)
    {
        uint id = i + 1;
        auto connections = fully_connected_map[id].GetConnections();

        if (connections.Top != 0)
        {
            sorted_edges[sorted_edges_size].from = id; 
            sorted_edges[sorted_edges_size].to = connections.Top;
            sorted_edges_size++;
        }
        
        if (connections.Right != 0)
        {
            sorted_edges[sorted_edges_size].from = id; 
            sorted_edges[sorted_edges_size].to = connections.Right;
            sorted_edges_size++;
        }

        if (connections.Bottom != 0)
        {
            sorted_edges[sorted_edges_size].from = id; 
            sorted_edges[sorted_edges_size].to = connections.Bottom;
            sorted_edges_size++;
        }

        if (connections.Left != 0)
        {
            sorted_edges[sorted_edges_size].from = id; 
            sorted_edges[sorted_edges_size].to = connections.Left;
            sorted_edges_size++;
        }
    }


    // ...and sort edges randomly
    _GenerateRandomDataToBuffer(random_data, RANDOM_DATA_SIZE);
    qsort(sorted_edges, sorted_edges_size, sizeof(Edge), _SortEdgesRandomly);
    CORE_DebugInfo("Sorted edges count: %u\n", sorted_edges_size);

    // create disjoint set from edges and 
    // union all edges according to Kruskal's algo
    core::DisjointSet   disjoint_set_handle(vertex_count + 1);
    mst_edges_size = 0;
    
    for (uint i = 0; i < sorted_edges_size; i++) {
        if (!disjoint_set_handle.Union(sorted_edges[i].from, sorted_edges[i].to)) {
            continue;
        }

        mst_edges[mst_edges_size].from = sorted_edges[i].from;
        mst_edges[mst_edges_size].to = sorted_edges[i].to;
        mst_edges_size++;
    }

    CORE_DebugInfo("Subsets after union: %u\n", disjoint_set_handle.GetSubsetsCount());
    CORE_DebugInfo("MST edges count: %u\n", mst_edges_size);


    // build new lab points map accordoing to mst_edges
    for (uint i = 0; i < vertex_count; i++)
    {
        uint id = i + 1;
        const PointConnections  connections = { 0, 0, 0, 0 };
        LabPoint                lab_point(id, connections); 
        const LabPoint          &temp_lab_point_handle = fully_connected_map[id];

        lab_point.SetAsExit(temp_lab_point_handle.IsExit());
        lab_point.SetAsSpawn(temp_lab_point_handle.IsSpawn());

        mst_points_map_handle[id] = lab_point;
    }

    for (uint i = 0; i < mst_edges_size; i++)
    {
        if ((mst_edges[i].from == 0) || (mst_edges[i].to == 0)) {
            continue;
        }

        _CopyConnectionsAccordingToEdge(fully_connected_map, mst_points_map_handle, mst_edges[i].from, mst_edges[i].to);
        _CopyConnectionsAccordingToEdge(fully_connected_map, mst_points_map_handle, mst_edges[i].to, mst_edges[i].from);
    }

    delete[] sorted_edges;
    delete[] mst_edges;
}

/*****************************************************************************************************************************/

static void _LabmapToJSON(const std::map<uint, LabPoint> &lab_map,
                          char                          **json)
{
#define RAW_JSON_OBJECT_MAX_SIZE    (120)
#define DEC_JSON_LEFT(SIZE_TO_DEC) (json_size_left = ((SIZE_TO_DEC) > json_size_left ? 0 : (json_size_left - (SIZE_TO_DEC)))) 

    int32               json_size_left;
    uint32              max_json;
    char                lab_point_raw_json_object[RAW_JSON_OBJECT_MAX_SIZE];
    uint32              lab_point_raw_json_object_char_count;


    uint map_size = lab_map.size();
    max_json = map_size * RAW_JSON_OBJECT_MAX_SIZE + 40;
    json_size_left = max_json;
    *json = new char[max_json + 1];
    *json[0] = '\0';

    lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "{\"count\": %u, \"points\": {", map_size);
    strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
    DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

    uint i = 0;
    for (auto& [_key, current_lab_point] : lab_map)
    {

        lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "\"%u\": ", current_lab_point.GetId());
        strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
        DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

        auto& connections = current_lab_point.GetConnections();
        lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object,
            "{\"top_id\": %u, \"right_id\": %u, \"bottom_id\": %u, \"left_id\": %u, \"is_exit\": %u, \"is_spawn\": %u}",
            connections.Top, connections.Right,
            connections.Bottom, connections.Left,
            current_lab_point.IsExit(), current_lab_point.IsSpawn());

        strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
        DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

        if (i != map_size - 1)
        {
            strncat(*json, ",", 1);
            DEC_JSON_LEFT(1);
        }

        i++;
    }

    strncat(*json, "}}", 2);
    DEC_JSON_LEFT(2);
}

static void _SaveToFile(const std::map<uint, LabPoint> &lab_map)
{
    char *json;
    _LabmapToJSON(lab_map, &json);

    FILE *FileToWrite = fopen("LAB_ROOMS.json", "w");
    fwrite(json, sizeof(char), strlen(json), FileToWrite);
    fclose(FileToWrite);

    delete[] json;
}

void lab::Generate(std::map<uint, LabPoint> &points_map, 
                   std::vector<uint>        &spawn_points)
{   
    // constexpr uint      lab_size              = MATRIX_SIZE * MATRIX_SIZE;
    std::map<uint, LabPoint> fully_connected_map;

    _BuildLab(fully_connected_map, spawn_points);
    _BuildMSTFrom(fully_connected_map, points_map);
    // _SaveToFile(points_map);
}

/*****************************************************************************************************************************/