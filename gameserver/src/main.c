#include "CORE.h"
#include "game-server/game-server.h"

static GameServer Server;

int main()
{
	GameServer_Create(&Server);
}