#include "modules/packet-processor/packet-processor.h"
#include "modules/packet-processor/packet-type.h"
#include "libs/utils/array.h"
#include "process-functions/process-functions.h"

/*****************************************************************************************************************************/

typedef struct PacketProcessor_s
{
    LabSession *const *sessions;
    uint               sessions_len;
    IOSystem          *io_system;
} PacketProcessor;


/*****************************************************************************************************************************/

PacketProcessStatusCode PacketProcessor_Process(PacketProcessor *packet_processor, 
                                                const Packet    *packet_in, 
                                                Packet          *packet_out)
{
    CORE_AssertPointer(packet_processor);
    CORE_AssertPointer(packet_in);
    CORE_AssertPointer(packet_out);


    if (packet_in->Type > PACKETTYPE_COUNT - 1) {
        CORE_DebugError("Wrong packet type. Got %u\n", packet_in->Type);
        return kPacketProcessStatusCode_BadInput;
    }

    const LabSession *session = PtrsArray_Find(LabSession,
                                               packet_processor->sessions,
                                               packet_processor->sessions_len,
                                               packet_in->SessionIndex);
    if (session == NULL) {
        CORE_DebugError("Session is NULL\n");
        return kPacketProcessStatusCode_BadInput;
    }

    packet_out->PayloadSize = 0;
    if (!ProcessFunctions_Get(packet_in->Type)(session, packet_in, packet_out)) {
        return kPacketProcessStatusCode_Error;
    }
    return kPacketProcessStatusCode_Ok;
}

/*****************************************************************************************************************************/

PacketProcessor *PacketProcessor_Create(LabSession *const *sessions, uint sessions_len, IOSystem *io_system)
{
    CORE_AssertPointer(sessions);
    CORE_AssertPointer(io_system);

    ProcessFunctions_Init();

    PacketProcessor *packet_processor = CORE_MemAlloc(sizeof(PacketProcessor), 1);
    packet_processor->sessions       = sessions;
    packet_processor->sessions_len   = sessions_len;
    packet_processor->io_system      = io_system;

    return packet_processor;
}

void PacketProcessor_Free(PacketProcessor *packet_processor)
{
    CORE_AssertPointer(packet_processor);


    CORE_MemFree(packet_processor);
}