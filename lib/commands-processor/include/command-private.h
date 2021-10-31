#ifndef _COMMAND_PRIVATE_H_
#define _COMMAND_PRIVATE_H_

#define _COMMAND_MAX_PAYLOAD_SIZE 	(512)

struct _Command
{
	uint32 		type;
	uint32 		payload_size;
	uint8 		payload[_COMMAND_MAX_PAYLOAD_SIZE];
};

#endif
