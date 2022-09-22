#ifndef _LAB_POINT_H_
#define _LAB_POINT_H_

#include <vector>
#include "modules/player/player.h"

struct PointConnections
{
    uint            Top; 
    uint            Right; 
    uint            Bottom; 
    uint            Left; 
};

class LabPoint
{
public:


    uint                        GetId() const;
    bool                        IsExit() const;
    bool                        IsSpawn() const;
    const PointConnections&     GetConnections() const;


    void SetId(uint id);
    void SetConnections(const PointConnections &connections);
    void SetAsExit(bool value);
    void SetAsSpawn(bool value);
    void Copy(const LabPoint &point);
    void AssignPlayer(const Player& player);


    explicit LabPoint() :
        m_id(0), 
        m_is_exit(false), 
        m_is_spawn(false) {};

    explicit LabPoint(uint id, const PointConnections connections) :
        m_id(id), 
        m_connections(connections), 
        m_is_exit(false), 
        m_is_spawn(false) {};

private:

    
    uint                m_id; 
    PointConnections    m_connections;
    bool                m_is_exit; 
    bool                m_is_spawn;

    std::vector<const Player *>   m_players;
};


#endif