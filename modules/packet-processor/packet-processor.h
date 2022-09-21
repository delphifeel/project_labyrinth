#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "core/core.h"
#include "libs/packet/packet.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"


typedef struct PacketProcessor_s PacketProcessor;
typedef enum PacketProcessStatusCode
{
    kPacketProcessStatusCode_Ok = 0,
    kPacketProcessStatusCode_Error,
    kPacketProcessStatusCode_BadInput,
} PacketProcessStatusCode;


PacketProcessStatusCode  PacketProcessor_Process(PacketProcessor   *packet_processor, 
                                                 const Packet      *packet_in,
                                                 Packet            *packet_out);

PacketProcessor *PacketProcessor_Create(LabSession *const *sessions, uint sessions_len, IOSystem *io_system);
void             PacketProcessor_Free(PacketProcessor *packet_processor);

#endif