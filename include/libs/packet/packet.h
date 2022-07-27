#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "CCORE.h"


typedef struct Packet_s Packet;


uint         Packet_GetType(const Packet *packet);
const uint8 *Packet_GetPlayerToken(const Packet *packet);
uint         Packet_GetPayloadSize(const Packet *packet);
const uint8 *Packet_GetPayload(const Packet *packet);
void         Packet_ToBuffer(const Packet *packet, uint8 **buffer_ptr, uint *buffer_len);
Packet      *Packet_Create(const uint8 buffer[], uint buffer_len);
void         Packet_Free(Packet *packet);

#endif