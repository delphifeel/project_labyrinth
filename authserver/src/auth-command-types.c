#include "commands-processor.h"
#include "authserver/auth-command-types.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 															\
	extern CORE_Bool _NAME(Command *command, Command *out_response_command) 					\

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandAuthenticate_Process);

static const CommandToProcessFunc _AuthServerCommandToProcessFunc[] = 
{
	{	kCommandType_Authenticate, CommandAuthenticate_Process	},
};

/*****************************************************************************************************************************/

const CommandToProcessFunc *GetAuthCommandToProcessFunc()
{
	return _AuthServerCommandToProcessFunc;
}

/*****************************************************************************************************************************/
