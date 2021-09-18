#include "CORE/CORE-system.h"
#include "CORE/CORE-file.h"

/*****************************************************************************************************************************/

CORE_Bool CORE_CreateUID(uint8 *out_uid)
{
	CORE_FileHandle file;
	uint32  		read_size;


	file = CORE_FileOpen("/dev/urandom", "r");
	if (file == NULL) 
	{
		return FALSE;
	}

	read_size = CORE_FileRead(out_uid, sizeof(uint8), UID_SIZE, file);
	if (read_size < UID_SIZE)
	{
		CORE_FileClose(file);
		return FALSE;
	}

	CORE_FileClose(file);
	return TRUE;
}

/*****************************************************************************************************************************/
