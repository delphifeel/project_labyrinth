#include "libs/packet/packet.h"
#include "modules/input-processor/input-processor.h"

/*****************************************************************************************************************************/

typedef struct InputProcessor_s
{
    LabSession **sessions;
    uint         sessions_len;
    IOSystem    *io_system;
} InputProcessor;

/*****************************************************************************************************************************/

bool InputProcessor_Process(InputProcessor *input_processor, const Packet *packet)
{

    return true;
}

/*****************************************************************************************************************************/

InputProcessor *InputProcessor_Create(LabSession **sessions, uint sessions_len, IOSystem *io_system)
{
    InputProcessor *input_processor = CORE_MemAlloc(sizeof(InputProcessor), 1);
    input_processor->sessions       = sessions;
    input_processor->sessions_len   = sessions_len;
    input_processor->io_system      = io_system;

    return input_processor;
}

void InputProcessor_Free(InputProcessor *input_processor)
{
    CORE_MemFree(input_processor);
}