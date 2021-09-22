#ifndef __COMMANDS_LISTENER_H__
#define __COMMANDS_LISTENER_H__

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
