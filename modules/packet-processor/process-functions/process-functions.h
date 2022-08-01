#ifndef _PROCESS_FUNCTIONS_H_
#define _PROCESS_FUNCTIONS_H_

#include "libs/packet/packet.h"
#include "modules/labyrinth/lab-session.h"

typedef bool (*ProcessPacketFunction)(const LabSession *session, const Packet *packet_in, Packet *packet_out);

void                  ProcessFunctions_Init(void);
ProcessPacketFunction ProcessFunctions_Get(uint type);

#endif