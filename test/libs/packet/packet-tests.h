#include "libs/packet/packet.h"

static void _CreatePacketFromInvalidBuffer(void)
{
    const uint8 small_buffer[] = {0x00, 0x00};
    Packet *packet = Packet_Create(small_buffer, sizeof(small_buffer));
    CORE_Assert(packet == NULL);

    const uint8 wrong_validation_header_buffer[] = { 
        0xBE, 0xBE, 0xBE, 0xAF,         // header
        0x00, 0x00, 0x00, 0x01,         // type

        0x00, 0x01, 0x02, 0x03,         // player token
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,

        0x01, 0x00, 0x00, 0x00,         // payload size
        0x66,                           // payload
    };
    packet = Packet_Create(wrong_validation_header_buffer, sizeof(wrong_validation_header_buffer));
    CORE_Assert(packet == NULL);
}

static void _CreatePacketFromValidBuffer(void)
{
    const uint8 valid_buffer[] = { 
        0xEF, 0xBE, 0xAD, 0xDE,         // header
        0x01, 0x00, 0x00, 0x00,         // type

        0x00, 0x01, 0x02, 0x03,         // player token
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,
        0x00, 0x01, 0x02, 0x03,

        0x01, 0x00, 0x00, 0x00,         // payload size
        0x66,                           // payload
    };
    Packet *packet = Packet_Create(valid_buffer, sizeof(valid_buffer));
    CORE_Assert(packet != NULL);

    CORE_Assert(Packet_GetType(packet) == 1);
    CORE_Assert(Packet_GetPayloadSize(packet) == 1);
    CORE_Assert(Packet_GetPayload(packet)[0] == 0x66);
}

void Packet_TestsRun(void)
{
    _CreatePacketFromInvalidBuffer();
    _CreatePacketFromValidBuffer();
}