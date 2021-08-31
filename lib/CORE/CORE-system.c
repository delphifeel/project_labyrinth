#include "../../include/CORE/CORE-system.h"
#include "../../include/CORE/CORE-file.h"

/*****************************************************************************************************************************/

CORE_Bool CORE_CreateUID(uint8 *OUT_UID)
{
	CORE_FileHandle File;
	uint32  		ReadSize;


	File = CORE_FileOpen("/dev/urandom", "r");
	if (File == NULL)
		return FALSE;

	ReadSize = CORE_FileRead(OUT_UID, sizeof(uint8), UID_SIZE, File);
	if (ReadSize < UID_SIZE)
	{
		CORE_FileClose(File);
		return FALSE;
	}

	CORE_FileClose(File);
	return TRUE;
}

/*****************************************************************************************************************************/