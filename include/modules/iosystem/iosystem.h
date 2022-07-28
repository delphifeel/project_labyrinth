#ifndef _IOSYSTEM_H_
#define _IOSYSTEM_H_

#include "CCORE.h"
#include "libs/packet/packet.h"

// TODO: to make it more testable pass types like `Reader` and `Writer` to this class

typedef struct IOSystem_s IOSystem;
typedef void (*IOSystemOnReadFunc)(const Packet *packet);


void        IOSystem_OnRead(IOSystem *ioSystem, IOSystemOnReadFunc on_read);
bool        IOSystem_Start(IOSystem *ioSystem);
IOSystem   *IOSystem_Create(void);
void        IOSystem_Free(IOSystem *ioSystem);

#endif