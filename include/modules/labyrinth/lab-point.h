#ifndef _LAB_POINT_H_
#define _LAB_POINT_H_

typedef struct LabPoint_s
{
    uint32          Id; 
    uint            top_connection_id; 
    uint            right_connection_id; 
    uint            bottom_connection_id; 
    uint            left_connection_id; 
    bool            is_exit; 
    bool            is_spawn;
} LabPoint;


#endif