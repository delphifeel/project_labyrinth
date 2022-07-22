#include <stddef.h>
#include "libs/packet/packet.h"


#define _PACKET_VALIDATION_HEADER (0xDEADBEEF)


typedef struct Packet_s
{
    uint32 type;
    uint8  player_token[PLAYER_TOKEN_SIZE];
    uint32 payload_size;
    uint8  *payload;
} Packet;

#define _PACKET_MIN_SIZE  (offsetof(Packet, payload))


// TODO: move to libCCORE
static uint8_t  _is_little_endian_test_buffer[2]    = {0x01, 0x00};
static int      _is_little_endian                   = -1;
static inline int _IsLittleEndian(void)
{
    if (_is_little_endian != -1)
        return _is_little_endian;
        
    _is_little_endian = (*(int *) _is_little_endian_test_buffer) == 1;
    return _is_little_endian;
}

#define _BytesToNumber(bytes, bytes_len, variable)  do {    \
    CORE_Assert(sizeof(variable) == bytes_len);             \
    variable = 0;                                           \
    if (_IsLittleEndian()) {                                \
        for (int i = 0; i < sizeof(variable); i++) {        \
            variable += bytes[i] << (i * 8);                \
        }                                                   \
    } else {                                                \
        for (int i = sizeof(variable) - 1; i >= 0; i--) {   \
            variable += bytes[i] << (i * 8);                \
        }                                                   \
    }                                                       \
} while (0)


static inline uint _CalcPacketSize(const Packet *packet)
{
    return offsetof(Packet, payload) + packet->payload_size;
}

static bool _ParsePacketFromBuffer(Packet *packet, const uint8 buffer[], uint buffer_len)
{
    CORE_AssertPointer(buffer);

    if (buffer_len < _PACKET_MIN_SIZE) {
        CORE_DebugError("Buffer is too small\n");
        return false;
    }

    const uint8 *buffer_ptr = buffer;

    // 0...4    (4 bytes)   - validation header
    uint32 validation_header;
    _BytesToNumber(buffer_ptr, sizeof(uint32), validation_header);
    if (validation_header != _PACKET_VALIDATION_HEADER) {
        CORE_DebugError("no validation header - `buffer` is not a packet\n");
        return false;
    }
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - type
    _BytesToNumber(buffer_ptr, sizeof(uint32), packet->type);
    buffer_ptr  += 4;

    // 8...40   (32 bytes) - player token
    CORE_MemCpy(packet->player_token, buffer_ptr, sizeof(packet->player_token));
    buffer_ptr += sizeof(packet->player_token);

    // 40...44   (4 bytes) - payload size
    _BytesToNumber(buffer_ptr, sizeof(uint32), packet->payload_size);
    buffer_ptr += 4;

    // 44...~   (~ bytes) - payload
    CORE_MemNewCopy(packet->payload, buffer_ptr, packet->payload_size);
    buffer_ptr += packet->payload_size;
    return true;
}


uint Packet_GetType(const Packet *packet)
{
    return packet->type;
}

const uint8 *Packet_GetPlayerToken(const Packet *packet)
{
    return packet->player_token;
}

uint Packet_GetPayloadSize(const Packet *packet)
{
    return packet->payload_size;
}

const uint8 *Packet_GetPayload(const Packet *packet)
{
    return packet->payload;
}

void Packet_ToBuffer(const Packet *packet, uint8 **buffer_ptr, uint *buffer_len)
{
    CORE_AssertPointer(packet);
    CORE_AssertPointer(buffer_ptr);

    *buffer_len     = _CalcPacketSize(packet);
    uint8 *buffer   = CORE_MemAlloc(sizeof(uint8), *buffer_len);
    *buffer_ptr     = buffer;

    // 0...4    (4 bytes)   - validation header
    uint32 *validation_header = (uint32 *) buffer;
    *validation_header = _PACKET_VALIDATION_HEADER;
    buffer += 4;

    // 4...8   (4 bytes)  - type
    uint32 *type = (uint32 *) buffer;
    *type = packet->type;
    buffer += 4;

    // 8...40   (32 bytes) - player token
    CORE_MemCpy(buffer, packet->player_token, sizeof(packet->player_token));
    buffer += 32;

    // 40...44   (4 bytes) - payload size
    uint32 *payload_size = (uint32 *) buffer;
    *payload_size = packet->payload_size;
    buffer += 4;

    // 44...~   (~ bytes) - payload
    CORE_MemCpy(buffer, packet->payload, packet->payload_size);
    buffer += packet->payload_size;
}

Packet *Packet_Create(const uint8 buffer[], uint buffer_len)
{
    Packet *packet = CORE_MemCalloc(sizeof(Packet), 1);
    if (!_ParsePacketFromBuffer(packet, buffer, buffer_len)) {
        return NULL;
    }

    return packet;
}

void Packet_Free(Packet *packet)
{
    CORE_MemFree(packet->payload);

    CORE_MemFree(packet);
}