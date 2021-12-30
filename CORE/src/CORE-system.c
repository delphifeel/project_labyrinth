#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "CORE/CORE-system.h"
#include "CORE/CORE-file.h"
#include "CORE/CORE-debug.h"

/*****************************************************************************************************************************/

CORE_Bool CORE_GenerateRandomDataToBuffer(uint8 buffer[], uint32 buffer_size)
{
	CORE_FileHandle 	file;
	uint32  			read_size;


	file = CORE_FileOpen("/dev/urandom", "re");
	if (file == NULL) 
	{
		return FALSE;
	}

	read_size = CORE_FileRead(buffer, sizeof(uint8), buffer_size, file);
	if (read_size != buffer_size)
	{
		CORE_FileClose(file);
		return FALSE;
	}

	CORE_FileClose(file);
	return TRUE;
}

CORE_Bool CORE_GenerateUIDToBuffer(uint8 buffer[UID_SIZE])
{
	return CORE_GenerateRandomDataToBuffer(buffer, UID_SIZE);
}

void CORE_Sleep(uint32 seconds)
{
	sleep(seconds);
}

/*****************************************************************************************************************************/
