#include "CORE.h"
#include "gameserver/game-server.h"

static GameServer Server;

int main(void)
{
	GameServer_Create(&Server);
	GameServer_Setup(Server);

	return 0;
}