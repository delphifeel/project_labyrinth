#ifndef _AUTH_IO_SYSTEM_H_
#define _AUTH_IO_SYSTEM_H_

#include "CCORE.h"
/*****************************************************************************************************************************/

/*
 * 		Auth Input Output System
 *      Client/Server communication using `AuthStruct` objects
 */

typedef struct AuthIOSystem_s *AuthIOSystem;

/*****************************************************************************************************************************/


void AuthIOSystem_Setup(AuthIOSystem instance); 
void AuthIOSystem_Start(AuthIOSystem instance); 

void AuthIOSystem_Create(AuthIOSystem *instance_ptr);
void AuthIOSystem_Free(AuthIOSystem *instance_ptr); 

/*****************************************************************************************************************************/

#endif
