#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/core.h"

class LabPoint;

struct Player
{
public:
    uint                GetId()             const { return m_id; }
    const LabPoint*     GetAssignedPoint()  const { return m_assigned_point; }

    void AssignPoint(const LabPoint* point);

    explicit Player(uint id) : 
        m_id(id),
        m_assigned_point(nullptr) {}

private:
    uint                m_id;
    // TODO: change to std::optional
    const LabPoint*     m_assigned_point;
};

#endif