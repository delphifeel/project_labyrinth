#include "commands-processor.h"
#include "gameserver/gameserver-command-types.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 															\
	extern CORE_Bool _NAME(Command *command, Command *out_response_command) 					\

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandPlayerMove_Process);
_DEFINE_PROCESS_FUNC(CommandStartGame_Process);

static const CommandToProcessFunc _command_to_process_func[] =
{
	{	kCommandType_PlayerMove, 	CommandPlayerMove_Process	},
	{	kCommandType_StartGame, 	CommandStartGame_Process	},
};

/*****************************************************************************************************************************/

const CommandToProcessFunc *GetGameServerCommandToProcessFunc()
{
	return _command_to_process_func;
}

/*****************************************************************************************************************************/
