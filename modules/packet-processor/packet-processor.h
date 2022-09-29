#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include <array>
#include <memory>
#include "include/config.h"
#include "libs/core/core.h"
#include "libs/packet/packet.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"
#include "process-functions/process-functions.h"


class PacketProcessor
{
public:
    enum Status
    {
        Ok = 0,
        Error,
        BadInput,
    };

    explicit PacketProcessor() :
            m_sessions(nullptr) {};

    void Setup(const std::array<LabSession *, SESSIONS_CAPACITY>  &sessions);

    PacketProcessor::Status  Process(const Packet& packet_in, Packet* packet_out) const;
private:
    const std::array<LabSession *, SESSIONS_CAPACITY>       *m_sessions;
};


#endif