#ifndef _CORE_TCP_SERVER_H_
#define _CORE_TCP_SERVER_H_

#include "CORE-object.h"
#include "CORE-types.h"
#include "CORE-debug.h"

/*****************************************************************************************************************************/

#define UID_SIZE 	(16)

CORE_OBJECT_DEFINE(CORE_TCPServer);

/*****************************************************************************************************************************/

void  	CORE_TCPServer_Setup(CORE_TCPServer instance);

void 	CORE_TCPServer_Create(CORE_TCPServer *instance_ptr);
void 	CORE_TCPServer_Free(CORE_TCPServer *instance_ptr);

/*****************************************************************************************************************************/

#endif
