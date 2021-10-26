#ifndef _COMMAND_PRIVATE_H_
#define _COMMAND_PRIVATE_H_

#define _COMMAND_MAX_PAYLOAD_SIZE 	(256)

typedef struct _CommandHeader
{
	uint32 			type;
	uint32			player_index;
	uint32			session_index;
	uint8 			player_token[TOKEN_SIZE];
} _CommandHeader;

struct _Command
{
	CORE_Bool 			is_header_set;
	_CommandHeader		header;
	uint32 				payload_size;
	uint8 				payload[_COMMAND_MAX_PAYLOAD_SIZE];
};

#endif
