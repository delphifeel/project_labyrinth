#ifndef _IOSYSTEM_H_
#define _IOSYSTEM_H_

#include "CCORE.h"
#include "libs/packet/packet.h"

// TODO: to make it more testable pass types like `Reader` and `Writer` to this class

typedef struct IOSystem_s  IOSystem;
typedef        void       *IOStream;
typedef void (*IOSystemOnReadFunc)(IOStream ioStream, const uint8 data[], uint data_len);


void        IOSystem_OnRead(IOSystem *ioSystem, IOSystemOnReadFunc on_read);
void        IOSystem_Write(const IOSystem *ioSystem, IOStream ioStream, const uint8 data[], uint data_len);
bool        IOSystem_Start(IOSystem *ioSystem);
IOSystem   *IOSystem_Create(uint32 data_start_flag);
void        IOSystem_Free(IOSystem *ioSystem);

#endif