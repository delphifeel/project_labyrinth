#include "chunk-splitter.h"
#include "bytes-to-number.h"


bool GetNextChunk(const uint8   buffer[], 
                  uint32        buffer_len,
                  const uint8   **chunk_ptr,
                  uint32        *chunk_size,
                  uint8         chunk_end_flag)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(chunk_ptr);

    uint8        end_flag;
    const uint8 *buffer_ptr         = buffer;
    uint32       buffer_len_left    = buffer_len;
    uint32       size               = 0;

    *chunk_ptr = buffer;

    while (buffer_len_left > 0) {
        if (buffer_len_left <= sizeof(end_flag)) {
            size += buffer_len_left;
            break;
        }
        
        end_flag = *buffer_ptr;
        if (end_flag == chunk_end_flag) {
            if (*chunk_ptr != NULL) {
                break;
            }

            *chunk_ptr = buffer_ptr;
            size = sizeof(end_flag);
            buffer_len_left -= sizeof(end_flag);
            buffer_ptr += sizeof(end_flag);
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
