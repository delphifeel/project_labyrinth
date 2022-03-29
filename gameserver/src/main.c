#include "CCORE.h"
#include "gameserver/game-server.h"

static GameServer Server;

int main(void)
{
	setbuf(stdout, NULL);
	GameServer_Create(&Server);
	GameServer_Setup(Server);

	return 0;
}