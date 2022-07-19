#ifndef _COMMANDS_IO_SYSTEM_
#define _COMMANDS_IO_SYSTEM_

#include "CCORE.h"
#include "lib/commands-processor/command.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

/*
 * 		Commands Input Output System
 *      Client/Server communication using `CommandStruct` objects
 */

typedef struct CommandsIOSystem_s *CommandsIOSystem;

/*****************************************************************************************************************************/

void CommandsIOSystem_Setup(CommandsIOSystem instance, LabSession sessions[], uint32 sessions_size);
void CommandsIOSystem_Start(CommandsIOSystem instance);

void CommandsIOSystem_Create(CommandsIOSystem *instance_ptr);
void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr);

/*****************************************************************************************************************************/

#endif
