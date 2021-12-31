#include "gameserver/gameserver-command-types.h"
#include "lib/commands-processor/commands-processor.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 																	\
	extern CORE_Bool _NAME(	struct GameServerCommand 			*command, 								\
							struct GameServerCommandResponse 	*out_response_command,					\
							CORE_Bool 							*out_is_have_response) 					\

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandPlayerMove_Process);
_DEFINE_PROCESS_FUNC(CommandStartGame_Process);
_DEFINE_PROCESS_FUNC(CommandPlayerInit_Process);

static const GameServerCommandsProcessor_CommandToProcessFunc _command_to_process_func[] =
{
	{	kCommandType_PlayerMove, 	CommandPlayerMove_Process	},
	{	kCommandType_StartGame, 	CommandStartGame_Process	},
	{	kCommandType_PlayerInit, 	CommandPlayerInit_Process	},
};

/*****************************************************************************************************************************/

const GameServerCommandsProcessor_CommandToProcessFunc *GetGameServerCommandToProcessFunc()
{
	return _command_to_process_func;
}

uint32 GetGameServerCommandToProcessFuncSize()
{
	return sizeof(_command_to_process_func) / sizeof(GameServerCommandsProcessor_CommandToProcessFunc);
}

/*****************************************************************************************************************************/
