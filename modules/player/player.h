#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <optional>
#include "libs/core/core.h"

class LabPoint;

class Player
{
public:
    uint                              GetId()             const { CORE_Assert(m_id != 0); return m_id; }
    std::optional<const LabPoint *>   GetAssignedPoint()  const { return m_assigned_point; }

    void                              SetId(uint id)            { m_id = id; };

    void AssignPoint(const LabPoint& point);

    explicit Player() : 
        m_id(0),
        m_assigned_point(std::nullopt) {}

private:
    uint                              m_id;
    std::optional<const LabPoint *>   m_assigned_point;
};

#endif