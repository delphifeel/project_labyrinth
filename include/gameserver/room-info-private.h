#ifndef _ROOM_INFO_PRIVATE_H_
#define _ROOM_INFO_PRIVATE_H_

#include "config.h"

struct RoomInfo
{
    uint32                  has_top_connection; 
    uint32                  has_right_connection; 
    uint32                  has_bottom_connection; 
    uint32                  has_left_connection; 
    uint32                  is_exit; 
    uint32                  is_spawn;
    uint32                  players_count;
    PlayerInfo              players_info[SESSION_PLAYERS_COUNT];
};

#endif