#include "CORE.h"
#include "gameserver/game-server.h"

static GameServer Server;

int main()
{
	GameServer_Create(&Server);
}