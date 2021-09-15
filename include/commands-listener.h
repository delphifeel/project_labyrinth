#ifndef __COMMANDS_LISTENER_H__
#define __COMMANDS_LISTENER_H__

#include "CORE.h"
#include "command.h"
#include "labyrinth/lab-session.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(CommandsListener);

/*****************************************************************************************************************************/

CORE_Bool CommandsListener_Process(CommandsListener, CommandStruct *CommandToProcess);
void CommandsListener_Setup(CommandsListener Instance, LabSession *Sessions, uint32 SessionSize);

void CommandsListener_Create(CommandsListener*);
void CommandsListener_Free(CommandsListener*);

/*****************************************************************************************************************************/

#endif