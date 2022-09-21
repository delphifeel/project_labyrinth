#include "chunk-splitter.h"
#include "bytes-to-number.h"


bool GetNextChunk(const uint8   buffer[], 
                  uint32        buffer_len,
                  const uint8   **chunk_ptr,
                  uint32        *chunk_size,
                  uint32        chunk_start_flag)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(chunk_ptr);

    // check min chunk_ptr size
    if (buffer_len <= sizeof(chunk_start_flag)) {
        return false;
    }

    uint32       start_flag;
    const uint8 *buffer_ptr         = buffer;
    uint32       buffer_len_left    = buffer_len;
    uint32       size               = 0;

    *chunk_ptr = NULL;

    while (buffer_len_left > 0) {
        if (buffer_len_left <= sizeof(start_flag)) {
            size += buffer_len_left;
            break;
        }
        BytesToNumber(buffer_ptr, buffer_len_left, start_flag);

        if (start_flag == chunk_start_flag) {
            if (*chunk_ptr != NULL) {
                break;
            }

            *chunk_ptr = buffer_ptr;
            size = sizeof(start_flag);
            buffer_len_left -= sizeof(start_flag);
            buffer_ptr += sizeof(start_flag);
        }

        size++;
        buffer_ptr++;
        buffer_len_left--;
    }

    if (*chunk_ptr == NULL) {
        return false;
    }  

    *chunk_size = size; 
    return true;
}
