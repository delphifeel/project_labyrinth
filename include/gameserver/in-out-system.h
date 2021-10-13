#ifndef _IN_OUT_SYSTEM_H_
#define _IN_OUT_SYSTEM_H_

#include "CORE.h"

/*****************************************************************************************************************************/

/*
 * 		InOutSystem - read/write system for any client-server side communication.
 */

CORE_OBJECT_DEFINE(InOutSystem);

typedef void (*OnReadFunc)(void *context, uint8 data[], uint32 data_size);

/*****************************************************************************************************************************/

void InOutSystem_OnRead(InOutSystem instance, OnReadFunc OnReadCallback);

void InOutSystem_Write(InOutSystem instance, uint8 data[], uint32 data_size);

void InOutSystem_Setup(InOutSystem instance, void *context);

void InOutSystem_Create(InOutSystem *instance_ptr);
void InOutSystem_Free(InOutSystem *instance_ptr);

/*****************************************************************************************************************************/

#endif
