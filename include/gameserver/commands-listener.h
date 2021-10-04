#ifndef _COMMANDS_LISTENER_H_
#define _COMMANDS_LISTENER_H_

#include "CORE.h"
#include "command.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(CommandsListener);

/*****************************************************************************************************************************/

CORE_Bool CommandsListener_Process(CommandsListener instance, CommandStruct *command_to_process);
void CommandsListener_Setup(CommandsListener instance, LabSession *sessions, uint32 sessions_size);

void CommandsListener_Create(CommandsListener* instance_ptr);
void CommandsListener_Free(CommandsListener* instance_ptr);

/*****************************************************************************************************************************/

#endif
