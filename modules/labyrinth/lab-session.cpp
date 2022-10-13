#include "libs/core/core.h"

#include "lab-generation.h"
#include "lab-session.h"


inline bool LabSession::_NoMoreSpawnPoints() const
{
    return m_assigned_spawn_points_count == m_spawn_points.size();
}



bool LabSession::IsReadyForStart() const
{
    return IsFull();
}

bool LabSession::IsFull() const
{
    return m_players_map.size() == m_max_players;
}

LabPointsMap& LabSession::GetLabMap()
{
    CORE_Assert(m_session_started);

    return m_lab_map;
}

const Player*
LabSession::FindPlayer(uint player_id) const
{
    if ( m_players_map.count(player_id) == 0 ) {
        return nullptr;
    }

    return &m_players_map.at(player_id);
}

std::pair<uint, bool>
LabSession::AddPlayer()
{
    CORE_Assert(m_session_started == false);

    if (IsFull()) {
        CORE_DebugError("No more free spots - session is FULL\n");
        return { 0, false };
    }

    if (_NoMoreSpawnPoints()) {
        CORE_DebugError("No more spawn points available\n");
        return { 0, false };
    }

    uint player_spawn_point_id = m_spawn_points[m_assigned_spawn_points_count++];
    auto point = m_lab_map.GetPointByID(player_spawn_point_id);
    if (!point) {
        CORE_DebugError("Point not found\n");
        return { 0, false };
    }

    uint added_player_id = m_players_map.size() + 1;
    m_players_map.emplace(added_player_id, Player { added_player_id });
    Player& new_player = m_players_map.at(added_player_id);
    new_player.AssignPoint(*point);
    point->AssignPlayer(new_player);

    CORE_DebugInfo(
        "Player #%u added. Spawn point id: %u\n", 
        added_player_id,
        player_spawn_point_id
    );
    return { added_player_id, true };
}

void LabSession::Start()
{
    CORE_Assert(IsReadyForStart());
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
    m_lab_map.Generate(m_spawn_points);
}
