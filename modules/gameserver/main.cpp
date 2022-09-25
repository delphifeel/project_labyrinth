#include "gameserver.h"

GameServer *gameserver;

int main()
{
    gameserver = new GameServer();
    gameserver->Start();
    return 0;
}