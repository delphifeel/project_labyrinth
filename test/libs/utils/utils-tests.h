#include "libs/utils/chunk-splitter.h"

static void _ChunkSplitterTests(void)
{
    const uint8     *chunk       = NULL;
    uint32           chunk_size  = 0;
    const uint32     chunk_start_flag   = 0xBEEFBEAF;
    const uint8     *buffer_ptr  = NULL;
    const uint8     *buffer_end  = NULL;
    uint32           buffer_left = 0;


    //  *** INVALID BUFFER ***
    
    // no chunk if buffer is too small
    const uint8 small_buffer[] = {0x00, 0x00};
    CORE_Assert(GetNextChunk(small_buffer, sizeof(small_buffer), &chunk, &chunk_size, chunk_start_flag) == false);

    // no chunk if no start flag in buffer
    const uint8 no_flag_buffer[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    };
    CORE_Assert(GetNextChunk(no_flag_buffer, sizeof(no_flag_buffer), &chunk, &chunk_size, chunk_start_flag) == false);

    //  *** VALID BUFFER ***

    // have 1 chunk_start_flag at start
    const uint8 valid_buffer_1[] = {
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x11, 0x22
    };
    CORE_Assert(GetNextChunk(valid_buffer_1, sizeof(valid_buffer_1), &chunk, &chunk_size, chunk_start_flag) == true);
    CORE_AssertIntEqual(chunk_size, 6);
    CORE_AssertIntEqual(chunk[4], 0x11);
    CORE_AssertIntEqual(chunk[5], 0x22);
    
    // have 2 chunk_start_flag: at start and in middle
    const uint8 valid_buffer_2[] = {
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x11, 0x22,
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x33,
    };

    buffer_ptr  = valid_buffer_2;
    buffer_end  = valid_buffer_2 + sizeof(valid_buffer_2);
    buffer_left = sizeof(valid_buffer_2);

    CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
    CORE_AssertIntEqual(chunk_size, 6);
    CORE_AssertIntEqual(chunk[4], 0x11);
    CORE_AssertIntEqual(chunk[5], 0x22);

    buffer_ptr = chunk + chunk_size;
    buffer_left = buffer_end - buffer_ptr;

    CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
    CORE_AssertIntEqual(chunk_size, 5);
    CORE_AssertIntEqual(chunk[4], 0x33);

    // have 2 chunk_start_flag: both in middle
    const uint8 valid_buffer_3[] = {
        0x00, 0xAA, 0xFF,
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x44, 0x55,
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x77,
    };

    buffer_ptr  = valid_buffer_3;
    buffer_left = sizeof(valid_buffer_3);
    buffer_end  = valid_buffer_3 + sizeof(valid_buffer_3);

    CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
    CORE_AssertIntEqual(chunk_size, 6);
    CORE_AssertIntEqual(chunk[4], 0x44);
    CORE_AssertIntEqual(chunk[5], 0x55);

    buffer_ptr = chunk + chunk_size;
    buffer_left = buffer_end - buffer_ptr;

    CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
    CORE_AssertIntEqual(chunk_size, 5);
    CORE_AssertIntEqual(chunk[4], 0x77);

    // have 2 chunk_start_flag: both in middle but wrong buffer size pass
    const uint8 valid_buffer_4[] = {
        0x00, 0xAA, 0xFF, 0xCC, 0xBB, 0xDD,
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x44, 0x55,
        0xAF, 0xBE, 0xEF, 0xBE,     // chunk_start_flag
        0x77,
    };
    CORE_Assert(GetNextChunk(valid_buffer_4, 6, &chunk, &chunk_size, chunk_start_flag) == false);
}

void Utils_TestsRun(void)
{
    _ChunkSplitterTests();
}