#include "bytes-to-number.h"

static uint8  _is_little_endian_test_buffer[2]    = {0x01, 0x00};
static int    _is_little_endian                   = -1;

int IsLittleEndian(void)
{
    if (_is_little_endian != -1)
        return _is_little_endian;
        
    _is_little_endian = (*(int *) _is_little_endian_test_buffer) == 1;
    return _is_little_endian;
}