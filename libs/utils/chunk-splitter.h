#ifndef _CHUNK_SPLITTER_H_
#define _CHUNK_SPLITTER_H_

#include "libs/core/core.h"

bool GetNextChunk(const uint8   buffer[], 
                  uint32        buffer_len,
                  const uint8   **chunk_ptr,
                  uint32        *chunk_size,
                  uint8         chunk_end_flag);

#endif