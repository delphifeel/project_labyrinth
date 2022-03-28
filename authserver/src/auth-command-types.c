#include "lib/commands-processor/commands-processor.h"
#include "authserver/auth-command-types.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 																\
	extern bool _NAME(	struct Command 	*command, 												\
							struct Command 	*out_response_command,									\
							bool 		*out_is_have_response) 									\

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandAuthenticate_Process);
_DEFINE_PROCESS_FUNC(CommandJoinLobby_Process);

static const AuthCommandsProcessor_CommandToProcessFunc _AuthServerCommandToProcessFunc[] = 
{
	{	kCommandType_Authenticate, 	CommandAuthenticate_Process	},
	{	kCommandType_JoinLobby, 	CommandJoinLobby_Process	},
};

/*****************************************************************************************************************************/

const AuthCommandsProcessor_CommandToProcessFunc *GetAuthCommandToProcessFunc()
{
	return _AuthServerCommandToProcessFunc;
}

uint32 GetAuthCommandToProcessFuncSize()
{
	return (sizeof(_AuthServerCommandToProcessFunc) / sizeof(AuthCommandsProcessor_CommandToProcessFunc));
}

/*****************************************************************************************************************************/
