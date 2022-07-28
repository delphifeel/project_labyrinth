#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"


typedef struct PacketProcessor_s PacketProcessor;


bool            PacketProcessor_Process(PacketProcessor *packet_processor, const Packet *packet);

PacketProcessor *PacketProcessor_Create(LabSession **sessions, uint sessions_len, IOSystem *io_system);
void            PacketProcessor_Free(PacketProcessor *packet_processor);

#endif