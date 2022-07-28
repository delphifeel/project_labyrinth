#include "libs/packet/packet.h"
#include "modules/packet-processor/packet-processor.h"

/*****************************************************************************************************************************/

typedef struct PacketProcessor_s
{
    LabSession **sessions;
    uint         sessions_len;
    IOSystem    *io_system;
} PacketProcessor;

/*****************************************************************************************************************************/

bool PacketProcessor_Process(PacketProcessor *packet_processor, const Packet *packet)
{

    return true;
}

/*****************************************************************************************************************************/

PacketProcessor *PacketProcessor_Create(LabSession **sessions, uint sessions_len, IOSystem *io_system)
{
    PacketProcessor *packet_processor = CORE_MemAlloc(sizeof(PacketProcessor), 1);
    packet_processor->sessions       = sessions;
    packet_processor->sessions_len   = sessions_len;
    packet_processor->io_system      = io_system;

    return packet_processor;
}

void PacketProcessor_Free(PacketProcessor *packet_processor)
{
    CORE_MemFree(packet_processor);
}