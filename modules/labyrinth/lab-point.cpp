#include <algorithm>
#include "lab-point.h"

uint LabPoint::GetId() const
{
    CORE_Assert(m_id != 0);
    
    return m_id;
}

bool LabPoint::IsExit() const
{
    return m_is_exit;
}

bool LabPoint::IsSpawn() const
{
    return m_is_spawn;
}

const PointConnections& LabPoint::GetConnections() const
{
    return m_connections;
}


void LabPoint::SetConnections(const PointConnections& connections)
{
    m_connections = connections;
}

void LabPoint::SetAsExit(bool value)
{
    m_is_exit = value;
}

void LabPoint::SetAsSpawn(bool value)
{
    m_is_spawn = value;
}

void LabPoint::AssignPlayer(const Player& player)
{
    // already in list ?
    if (std::find(m_players.begin(), m_players.end(), &player) != m_players.end()) {
        return;
    }

    m_players.push_back(&player);
}