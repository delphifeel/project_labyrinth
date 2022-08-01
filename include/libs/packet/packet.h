#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "CCORE.h"

typedef struct Packet_s
{
    uint          Type;
    uint          SessionIndex;
    uint          PlayerId;
    uint          PayloadSize;
    const uint8  *Payload;
} Packet;

#endif