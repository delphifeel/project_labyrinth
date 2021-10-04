#ifndef _CORE_FILE_H_
#define _CORE_FILE_H_

#include <stdio.h>

#define CORE_FileOpen 		fopen
#define CORE_FileClose 		fclose
#define CORE_FileRead 		fread
#define CORE_FileWrite 		fwrite

typedef FILE* CORE_FileHandle;

#endif
