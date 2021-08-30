#ifndef __CORE_FILE_H__
#define __CORE_FILE_H__

#include <stdio.h>

#define CORE_FileOpen 		fopen
#define CORE_FileClose 		fclose
#define CORE_FileRead 		fread
#define CORE_FileWrite(FILE_HANDLE, ELEMENT_SIZE, NUMBER_OF_ELEMENTS, DATA_POINTER) \
	fwrite(DATA_POINTER, ELEMENT_SIZE, NUMBER_OF_ELEMENTS, FILE_HANDLE)

typedef FILE* CORE_FileHandle;

#endif