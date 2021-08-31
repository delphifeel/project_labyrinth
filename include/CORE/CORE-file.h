#ifndef __CORE_FILE_H__
#define __CORE_FILE_H__

#include <stdio.h>

#define CORE_FileOpen 		fopen
#define CORE_FileClose 		fclose
#define CORE_FileRead 		fread
#define CORE_FileWrite 		fwrite

typedef FILE* CORE_FileHandle;

#endif
