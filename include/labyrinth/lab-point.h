#ifndef __LAB_POINT_H__
#define __LAB_POINT_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

typedef struct ConnectionsStruct ConnectionsStruct;


typedef struct LabPointStruct {
	uint32			Id; 
	uint32 			TopConnectionId; 
	uint32 			RightConnectionId; 
	uint32 			BottomConnectionId; 
	uint32 			LeftConnectionId; 
	CORE_Bool 		IsExit; 
	CORE_Bool 		IsSpawn;
} LabPointStruct;


#endif
