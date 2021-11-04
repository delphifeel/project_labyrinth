#include "commands-processor.h"
#include "authserver/auth-command-types.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 																\
	extern CORE_Bool _NAME(	uint32 			command_type,											\
							struct Command 	*command, 												\
							struct Command 	*out_response_command,									\
							CORE_Bool 		*out_is_have_response) 									\

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandAuthenticate_Process);

static const AuthCommandsProcessor_CommandToProcessFunc _AuthServerCommandToProcessFunc[] = 
{
	{	kCommandType_Authenticate, CommandAuthenticate_Process	},
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
