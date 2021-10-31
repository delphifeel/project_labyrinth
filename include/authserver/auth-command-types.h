#ifndef _AUTH_COMMAND_TYPES_H_
#define _AUTH_COMMAND_TYPES_H_

enum 
{
	kCommandType_Authenticate,
};

const CommandToProcessFunc *GetAuthCommandToProcessFunc();

#endif
