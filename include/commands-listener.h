#ifndef __COMMANDS_LISTENER_H__
#define __COMMANDS_LISTENER_H__

#include "CORE.h"
#include "command.h"
#include "labyrinth/lab-session.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(CommandsListener);

/*****************************************************************************************************************************/

CORE_Bool CommandsListener_Process(CommandsListener, CommandStruct *command_to_process);
void CommandsListener_Setup(CommandsListener instance, LabSession *sessions, uint32 sessions_size);

void CommandsListener_Create(CommandsListener*);
void CommandsListener_Free(CommandsListener*);

/*****************************************************************************************************************************/

#endif
