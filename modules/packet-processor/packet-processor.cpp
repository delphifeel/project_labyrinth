#include <algorithm>
#include "process-functions/process-functions.h"
#include "packet-processor.h"
#include "packet-type.h"

/*****************************************************************************************************************************/

void PacketProcessor::Setup(const std::array<LabSession *, SESSIONS_CAPACITY>  &sessions)
{
    m_sessions  = &sessions;

    ProcessFunctions_Init();
}

PacketProcessor::Status 
PacketProcessor::Process(const Packet& packet_in, Packet* packet_out) const
{
    CORE_AssertPointer(m_sessions);
    CORE_AssertPointer(packet_out);

    if ( packet_in.Type > PacketType::END - 1 ) {
        CORE_DebugError("Wrong packet type. Got %u\n", packet_in.Type);
        return Status::BadInput;
    }

    if ( packet_in.SessionIndex > m_sessions->size() - 1 ) {
        CORE_DebugError("Invalid session index\n");
        return Status::BadInput;
    }
    auto& session = m_sessions->at(packet_in.SessionIndex);
    packet_out->PayloadSize = 0;

    if ( !ProcessFunctions_Get(packet_in.Type)(session, &packet_in, packet_out) ) {
        return Status::Error;
    }
    return Status::Ok;
}