#include "../packet-type.h"
#include "process-functions.h"

#define _DEFINE_FUNC(name) \
extern bool name(const LabSession *session, const Packet *packet_in, Packet *packet_out)


_DEFINE_FUNC(Process_JoinLobby);

static bool                  _inited = false;
static ProcessPacketFunction _process_functions[ PacketType::END ] = {NULL};

void ProcessFunctions_Init(void)
{
    CORE_Assert(_inited == false);
    _inited = true;

    // PROCESS FUNCTIONS HERE
    _process_functions[PacketType::JoinLobby] = Process_JoinLobby;
}

ProcessPacketFunction ProcessFunctions_Get(uint type)
{
    ProcessPacketFunction func = _process_functions[type];
    CORE_AssertPointer(func);
    return func;
}
