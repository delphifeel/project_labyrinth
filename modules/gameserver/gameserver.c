#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"
#include "modules/packet-processor/packet-processor.h"
#include "libs/packet/packet.h"
#include "config.h"

/*****************************************************************************************************************************/

typedef struct GameServer_s
{
    LabSession              *sessions[SESSIONS_CAPACITY];
    IOSystem                *io_system;
    PacketProcessor          *packet_processor;
} GameServer;

static GameServer *_gameserver;

/*****************************************************************************************************************************/

static void _PrepareSessions(void)
{
    CORE_MemZero(_gameserver->sessions, sizeof(_gameserver->sessions));
}

static void _FreeSessions(void)
{
    for (uint i = 0; i < SESSIONS_CAPACITY; i++) {
        if (_gameserver->sessions[i] != NULL) {
            LabSession_Free(_gameserver->sessions[i]);
        }
    }
}

static void _OnInputRead(const Packet *packet)
{
    PacketProcessor_Process(_gameserver->packet_processor, packet);
}

static void _StartGameServer(void)
{
    _gameserver = CORE_MemAlloc(sizeof(GameServer), 1);

    CORE_DebugInfo("Init sessions\n");
    _PrepareSessions();

    CORE_DebugInfo("Init in-out system\n");
    _gameserver->io_system = IOSystem_Create();
    IOSystem_OnRead(_gameserver->io_system, _OnInputRead);

    CORE_DebugInfo("Init input processor\n");
    uint sessions_len = sizeof(_gameserver->sessions) / sizeof(*_gameserver->sessions);
    _gameserver->packet_processor = PacketProcessor_Create(_gameserver->sessions, sessions_len, _gameserver->io_system);

    IOSystem_Start(_gameserver->io_system);
}

static void _FreeGameServer(void)
{
    PacketProcessor_Free(_gameserver->packet_processor);
    IOSystem_Free(_gameserver->io_system);
    _FreeSessions();

    CORE_MemFree(_gameserver);
}

/*****************************************************************************************************************************/

int main()
{
    _StartGameServer();
    return 0;
}