#ifndef _CHUNK_SPLITTER_H_
#define _CHUNK_SPLITTER_H_

#include "CCORE.h"

bool GetNextChunk(const uint8   buffer[], 
                  uint32        buffer_len,
                  const uint8   **chunk_ptr,
                  uint32        *chunk_size,
                  uint32        chunk_start_flag);

#endif