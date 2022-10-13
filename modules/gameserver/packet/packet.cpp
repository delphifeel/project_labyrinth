#include <cstring>
#include "libs/utils/bytes-to-number.h"
#include "packet.h"

constexpr uint _BUFFER_MIN_SIZE = 40;

static void _TokenPtrToArr(const uint8 token_ptr[], PlayerToken &arr)
{
    for (uint i = 0; i < kPlayerTokenSize; i++) {
        arr[i] = token_ptr[i];
    }
}

bool PacketIn::FromBuffer(const uint8    buffer[], 
                          uint           buffer_len)
{
    CORE_AssertPointer(buffer);

    if (buffer_len < _BUFFER_MIN_SIZE) {
        CORE_DebugError("Buffer is too small\n");
        return false;
    }

    const uint8 *buffer_ptr = buffer;

    // 0...4   (4 bytes)  - validation header
    uint32 header_value = 0;
    BytesToNumber(buffer_ptr, sizeof(header_value), header_value);
    ValidationHeader = header_value;
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - type
    uint32 type_value = 0;
    BytesToNumber(buffer_ptr, sizeof(type_value), type_value);
    Type = type_value;
    buffer_ptr += 4;

    // 4...40   (32 bytes) - player token
    _TokenPtrToArr(buffer_ptr, Token);
    buffer_ptr += kPlayerTokenSize;

    // 40...44   (4 bytes) - payload size
    uint32 payload_size_value = 0;
    BytesToNumber(buffer_ptr, sizeof(payload_size_value), payload_size_value);
    PayloadSize = payload_size_value;
    buffer_ptr += 4;

    // 44...~   (~ bytes) - payload
    Payload = buffer_ptr;
    buffer_ptr += payload_size_value;
    return true;
}

uint PacketOut::ToBuffer(uint8 buffer[], uint buffer_size)
{
    CORE_AssertPointer(buffer);

    uint8 *buffer_ptr = buffer;
    uint32 value_u32;

    // 0...4   (4 bytes)  - validation header
    value_u32 = ValidationHeader;
    memcpy(buffer_ptr, &value_u32, 4);
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - status
    value_u32 = Status;
    memcpy(buffer_ptr, &value_u32, 4);
    buffer_ptr += 4;

    // 8...12   (4 bytes)  - type
    value_u32 = Type;
    memcpy(buffer_ptr, &value_u32, 4);
    buffer_ptr += 4;

    // 12...44   (32 bytes) - player token
    memcpy(buffer_ptr, Token.data(), Token.size());
    buffer_ptr += Token.size();

    // 44...48   (4 bytes) - payload size
    value_u32 = PayloadSize;
    memcpy(buffer_ptr, &value_u32, 4);
    buffer_ptr += 4;

    // 48...~   (~ bytes) - payload
    if (PayloadSize > 0) {
        memcpy(buffer_ptr, Payload, PayloadSize);
        buffer_ptr += PayloadSize;
    }

    uint result = buffer_ptr - buffer;
    CORE_Assert(result <= buffer_size);
    return result;
}