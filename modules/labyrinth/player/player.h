#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <optional>
#include "libs/core/core.h"

class LabPoint;

class Player
{
public:
    Player(Player&) = delete;
    Player(Player&&) = default;

    uint                              GetId()             const { return m_id; }
    const LabPoint*                   GetAssignedPoint()  const { return m_assigned_point; }

    void MoveTo(LabPoint& point);

    explicit Player(uint id) : 
        m_id(id),
        m_assigned_point(nullptr) {}

private:
    uint                        m_id;
    LabPoint*                   m_assigned_point;
};

#endif