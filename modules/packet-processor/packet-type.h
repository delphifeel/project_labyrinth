#ifndef _PACKET_TYPE_H_
#define _PACKET_TYPE_H_

enum class PacketType
{
    JoinLobby = 0,

    END, // end flag to get enum values count
};

#define PACKETTYPE_COUNT   (PacketType::END)

#endif