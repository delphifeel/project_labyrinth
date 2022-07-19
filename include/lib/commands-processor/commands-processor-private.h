#ifndef _COMMANDS_PROCESSOR_PRIVATE_H_
#define _COMMANDS_PROCESSOR_PRIVATE_H_

#include "CCORE.h"

#define _COMMANDS_PROCESSOR_DEFINE(_NAME, _IN_COMMAND_TYPE, _OUT_COMMAND_TYPE)								\
																											\
typedef struct _NAME##_s *_NAME;																				\
																											\
typedef bool (*_NAME##CommandProcessFunc)(_IN_COMMAND_TYPE 	command, 										\
												_OUT_COMMAND_TYPE 	out_response_command,					\
												bool 			*out_is_have_response);						\
																											\
typedef struct _NAME##_CommandToProcessFunc																	\
{																											\
	uint32 						command_type;																\
	_NAME##CommandProcessFunc 	process_func;																\
} _NAME##_CommandToProcessFunc;																				\
																											\
/*********************************************************************************************/				\
																											\
bool 	_NAME##_Process(_NAME 				instance, 													\
							uint32 				command_type,												\
							_IN_COMMAND_TYPE 	command, 													\
							_OUT_COMMAND_TYPE 	out_response_command,										\
							bool 			*out_is_have_response);										\
																											\
void 		_NAME##_Setup(	_NAME 								instance, 									\
							const _NAME##_CommandToProcessFunc 	*command_to_process_func_array,				\
							uint32 								command_to_process_func_array_size);		\
																											\
void 		_NAME##_Create(_NAME *instance_ptr);															\
void 		_NAME##_Free(_NAME *instance_ptr);																\
	

#endif
