#ifndef _GAMESERVER_COMMAND_TYPES_H_
#define _GAMESERVER_COMMAND_TYPES_H_

enum 
{
	kCommandType_PlayerMove,
	kCommandType_StartGame,
};

const CommandToProcessFunc *GetGameServerCommandToProcessFunc();

#endif
