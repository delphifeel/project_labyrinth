#ifndef _AUTH_COMMAND_TYPES_H_
#define _AUTH_COMMAND_TYPES_H_

#include <commands-processor.h>
#include "auth-commands-processor.h"

enum 
{
	kCommandType_Authenticate,
};

enum
{
	kCommandResponseType_Authenticate,
};

const AuthCommandsProcessor_CommandToProcessFunc 	*GetAuthCommandToProcessFunc();
uint32 												GetAuthCommandToProcessFuncSize();

#endif
