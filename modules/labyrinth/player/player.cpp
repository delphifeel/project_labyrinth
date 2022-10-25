#include "../lab-point.h"
#include "player.h"

void Player::MoveTo(LabPoint& point)
{
    if (m_assigned_point) {
        // remove from prev point
        m_assigned_point->RemovePlayer(*this);
    }

    m_assigned_point = &point;
    point.AssignPlayer(*this);
}