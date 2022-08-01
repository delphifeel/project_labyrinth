#ifndef _PACKET_TYPE_H_
#define _PACKET_TYPE_H_

typedef enum PacketType
{
    kPacket_JoinLobby = 0,

    PacketTypeEnumEnd, // end flag to get enum values count
} PacketType;

#define PACKETTYPE_COUNT    (PacketTypeEnumEnd)

#endif