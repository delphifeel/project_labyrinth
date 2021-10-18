#ifndef _IN_OUT_SYSTEM_H_
#define _IN_OUT_SYSTEM_H_

#include "CORE.h"
#include "command.h"

/*****************************************************************************************************************************/

/*
 * 		Commands Input Output System
 *      Client/Server communication using `CommandStruct` objects
 */

CORE_OBJECT_DEFINE(CommandsIOSystem);

typedef void (*OnCommandGetFunc)(CommandsIOSystem instance, void *context, const CommandStruct *command_from_client);

/*****************************************************************************************************************************/

void CommandsIOSystem_OnGet(CommandsIOSystem instance, OnCommandGetFunc on_command_get);

void CommandsIOSystem_Send(CommandsIOSystem instance, const CommandStruct *command_to_client);

void CommandsIOSystem_SetContext(CommandsIOSystem instance, void *context);

void CommandsIOSystem_Setup(CommandsIOSystem instance);
void CommandsIOSystem_Start(CommandsIOSystem instance);

void CommandsIOSystem_Create(CommandsIOSystem *instance_ptr);
void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr);

/*****************************************************************************************************************************/

#endif
