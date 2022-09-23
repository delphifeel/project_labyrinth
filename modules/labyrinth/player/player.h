#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <optional>
#include "libs/core/core.h"

class LabPoint;

class Player
{
public:
    uint                              GetId()             const { return m_id; }
    const LabPoint*                   GetAssignedPoint()  const { return m_assigned_point; }

    void AssignPoint(const LabPoint& point);

    explicit Player(uint id) : 
        m_id(id),
        m_assigned_point(nullptr) {}

private:
    uint                              m_id;
    const LabPoint*                   m_assigned_point;
};

#endif