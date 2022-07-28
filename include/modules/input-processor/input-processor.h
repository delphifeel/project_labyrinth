#ifndef _INPUT_PROCESSOR_H_
#define _INPUT_PROCESSOR_H_

#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"


typedef struct InputProcessor_s InputProcessor;


bool            InputProcessor_Process(InputProcessor *input_processor, const Packet *packet);

InputProcessor *InputProcessor_Create(LabSession **sessions, uint sessions_len, IOSystem *io_system);
void            InputProcessor_Free(InputProcessor *input_processor);

#endif