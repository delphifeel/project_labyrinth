#ifndef _AUTH_COMMAND_TYPES_H_
#define _AUTH_COMMAND_TYPES_H_

#include "authserver/auth-commands-processor.h"

enum 
{
	kCommandType_Authenticate,
	kCommandType_JoinLobby,
};

enum
{
	kCommandResponseType_Authenticate,
	kCommandResponseType_JoinLobby,
};

const AuthCommandsProcessor_CommandToProcessFunc 	*GetAuthCommandToProcessFunc();
uint32 												GetAuthCommandToProcessFuncSize();

#endif
