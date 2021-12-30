#ifndef _CORE_SYSTEM_H_
#define _CORE_SYSTEM_H_

#include "CORE-types.h"

/*****************************************************************************************************************************/

#define UID_SIZE 	(16)

/*****************************************************************************************************************************/

CORE_Bool 	CORE_GenerateUIDToBuffer(uint8 buffer[UID_SIZE]);
CORE_Bool 	CORE_GenerateRandomDataToBuffer(uint8 buffer[], uint32 buffer_size);
void 		CORE_Sleep(uint32 seconds);

/*****************************************************************************************************************************/

#endif
