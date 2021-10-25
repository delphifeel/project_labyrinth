#ifndef _IN_OUT_SYSTEM_H_
#define _IN_OUT_SYSTEM_H_

#include "CORE.h"
#include "command.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

/*
 * 		Commands Input Output System
 *      Client/Server communication using `CommandStruct` objects
 */

CORE_OBJECT_DEFINE(CommandsIOSystem);

// typedef void (*OnCommandGetFunc)(CommandsIOSystem                   instance, 
//                                  void                               *context, 
//                                  const ClientCommandHeaderStruct    *command_header,
//                                  const uint8                        *command_payload);

/*****************************************************************************************************************************/

void CommandsIOSystem_Setup(CommandsIOSystem instance, LabSession sessions[], uint32 sessions_size);
void CommandsIOSystem_Start(CommandsIOSystem instance);

void CommandsIOSystem_Create(CommandsIOSystem *instance_ptr);
void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr);

/*****************************************************************************************************************************/

#endif
