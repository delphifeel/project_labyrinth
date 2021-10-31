#ifndef _AUTH_IO_SYSTEM_H_
#define _AUTH_IO_SYSTEM_H_

#include "CORE.h"
/*****************************************************************************************************************************/

/*
 * 		Auth Input Output System
 *      Client/Server communication using `AuthStruct` objects
 */

CORE_OBJECT_DEFINE(AuthIOSystem);

/*****************************************************************************************************************************/


void AuthIOSystem_Setup(AuthIOSystem instance); 
void AuthIOSystem_Start(AuthIOSystem instance); 

void AuthIOSystem_Create(AuthIOSystem *instance_ptr);
void AuthIOSystem_Free(AuthIOSystem *instance_ptr); 

/*****************************************************************************************************************************/

#endif
