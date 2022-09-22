#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_


#include <map>
#include <vector>
#include <utility>

#include "libs/core/core.h"
#include "modules/player/player.h"

#include "lab-points-map.h"

/*****************************************************************************************************************************/

class LabSession
{
public:
    std::pair<const Player *, bool>     FindPlayer(uint player_id) const;


    std::pair<uint, bool>               AddPlayer();
    void                                Start();
    void                                Setup(uint players_count);
    LabPointsMap&                       GetLabMap();


private:
    bool _NoMoreSpawnPoints() const;
    bool _IsFull() const;
    bool _IsReadyForStart() const;


    LabPointsMap            m_lab_map;
    std::map<uint, Player>  m_players_map;
    uint                    m_max_players;
    std::vector<uint>       m_spawn_points;
    uint                    m_assigned_spawn_points_count;
    bool                    m_session_started;
    /* etc */
};

/*****************************************************************************************************************************/

#endif