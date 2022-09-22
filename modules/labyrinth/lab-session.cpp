#include "libs/core/core.h"

#include "lab-generation.h"
#include "lab-session.h"


bool LabSession::_NoMoreSpawnPoints() const
{
    return m_assigned_spawn_points_count == m_spawn_points.size();
}

bool LabSession::_IsFull() const
{
    return m_players_map.size() == m_max_players;
}

bool LabSession::_IsReadyForStart() const
{
    return _IsFull();
}


LabPointsMap& LabSession::GetLabMap()
{
    CORE_Assert(m_session_started);

    return m_lab_map;
}

std::pair<const Player *, bool>
LabSession::FindPlayer(uint player_id) const
{
    if ( m_players_map.count(player_id) == 0 ) {
        return { nullptr, false };
    }

    return { &m_players_map.at(player_id), true };
}

std::pair<uint, bool>
LabSession::AddPlayer()
{
    CORE_Assert(m_session_started == false);

    if (_IsFull()) {
        CORE_DebugError("No more free spots - session is FULL\n");
        return { 0, false };
    }

    if (_NoMoreSpawnPoints()) {
        CORE_DebugError("No more spawn points available\n");
        return { 0, false };
    }

    uint player_spawn_point_id = m_spawn_points[m_assigned_spawn_points_count++];
    auto [ point, point_ok ] = m_lab_map.GetPointByID(player_spawn_point_id);
    if (!point_ok) {
        CORE_DebugError("Point not found\n");
        return { 0, false };
    }

    uint added_player_id = m_players_map.size() + 1;
    Player new_player;
    new_player.SetId(added_player_id);
    new_player.AssignPoint(*point);
    point->AssignPlayer(new_player);

    CORE_DebugInfo(
        "Player #%u added. Spawn point id: %u\n", 
        added_player_id,
        player_spawn_point_id
    );
    m_players_map[added_player_id] = new_player;
    return { added_player_id, true };
}

void LabSession::Start()
{
    CORE_Assert(_IsReadyForStart());
    CORE_Assert(m_session_started == false);

    m_session_started = true;
    CORE_DebugInfo("Session started\n");
}

void LabSession::Setup(uint players_count)
{
    CORE_Assert(players_count > 0);

    CORE_DebugInfo("Setup session. Player count: %u\n", players_count);

    m_max_players = players_count;
    m_session_started = false;
    m_assigned_spawn_points_count = 0;

    std::map<uint, LabPoint> points_map;
    lab::Generate(points_map, m_spawn_points);
    m_lab_map.SetMap(points_map);
}
