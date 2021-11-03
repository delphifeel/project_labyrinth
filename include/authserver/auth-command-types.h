#ifndef _AUTH_COMMAND_TYPES_H_
#define _AUTH_COMMAND_TYPES_H_

#include <commands-processor.h>

enum 
{
	kCommandType_Authenticate,
};

enum
{
	kCommandResponseType_Authenticate,
};

const CommandToProcessFunc *GetAuthCommandToProcessFunc();
uint32 						GetAuthCommandToProcessFuncSize();

#endif
