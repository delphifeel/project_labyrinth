// #include "libs/utils/chunk-splitter.h"

// static void _ChunkSplitterTests(void)
// {
//     const uint8     *chunk       = NULL;
//     uint32           chunk_size  = 0;
//     const uint8      chunk_start_flag   = 0x00;
//     const uint8     *buffer_ptr  = NULL;
//     const uint8     *buffer_end  = NULL;
//     uint32           buffer_left = 0;

//     //  *** VALID BUFFER ***

//     // if no start_flag - all is one chunk
//     const uint8 no_flag_buffer[] = {
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//         0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//     };
//     CORE_Assert(GetNextChunk(no_flag_buffer, sizeof(no_flag_buffer), &chunk, &chunk_size, chunk_start_flag) == true);
//     CORE_AssertIntEqual(chunk_size, 42);


//     // have 1 chunk_start_flag at start
//     const uint8 valid_buffer_1[] = {
//         0x11, 0x22
//     };
//     CORE_Assert(GetNextChunk(valid_buffer_1, sizeof(valid_buffer_1), &chunk, &chunk_size, chunk_start_flag) == true);
//     CORE_AssertIntEqual(chunk_size, 2);
//     CORE_AssertIntEqual(chunk[0], 0x11);
//     CORE_AssertIntEqual(chunk[1], 0x22);
    
//     // have 2 chunk_start_flag: at start and in middle
//     const uint8 valid_buffer_2[] = {
//         0x11, 0x22,
//         0x00,

//         0x33,
//         0x00
//     };

//     buffer_ptr  = valid_buffer_2;
//     buffer_end  = valid_buffer_2 + sizeof(valid_buffer_2);
//     buffer_left = sizeof(valid_buffer_2);

//     CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
//     CORE_AssertIntEqual(chunk_size, 3);
//     CORE_AssertIntEqual(chunk[0], 0x11);
//     CORE_AssertIntEqual(chunk[1], 0x22);

//     buffer_ptr = chunk + chunk_size;
//     buffer_left = buffer_end - buffer_ptr;

//     CORE_Assert(GetNextChunk(buffer_ptr, buffer_left, &chunk, &chunk_size, chunk_start_flag) == true);
//     CORE_AssertIntEqual(chunk_size, 2);
//     CORE_AssertIntEqual(chunk[0], 0x33);
// }

void Utils_TestsRun(void)
{
    // _ChunkSplitterTests();
}