#include "../include/commands-listener.h"

#define SESSION_SIZE 	(2)

static CommandsListener 	instance;
static LabSession 			sessions[SESSION_SIZE];
static uint32  				player1_index, player2_index;

void Init()
{
	LabSession_Create(&sessions[0]);
	LabSession_Setup(sessions[0], 1);
	LabSession_AddPlayer(sessions[0], "Player_1", &player1_index);

	LabSession_Create(&sessions[1]);
	LabSession_Setup(sessions[1], 1);
	LabSession_AddPlayer(sessions[1], "Player_2", &player2_index);

	CommandsListener_Create(&instance);	
	CommandsListener_Setup(instance, sessions, SESSION_SIZE);
}

void Free()
{
	LabSession_Free(&sessions[0]);
	LabSession_Free(&sessions[1]);
	CommandsListener_Free(&instance);	
}

void Test_CommandPlayerMove()
{
	uint32 			player1_directions_size = 2;
	MoveDirection 	player1_directions[] = {kMoveDirection_Top, kMoveDirection_Left};
	uint32  		session_index = 0;
	CommandStruct 	command_player_move = {
		.type 				= kCommandType_PlayerMove,
		.player_index 		= player1_index,
		.session_index 		= session_index,
		.player_move_payload = {
			player1_directions_size,
			player1_directions,
		} 
	};
	Player 					player1;
	uint32  				player1_position_point_id, predicted_new_position_point_id;
	LabPointStruct 			player1_point;
	LabPointsMapReader  	lab_points_reader;
	CORE_Bool 				is_player_moved;


	LabSession_FindPlayer(sessions[session_index], player1_index, &player1);
	Player_GetPositionPointId(player1, &player1_position_point_id);

	LabSession_GetLabPointsReader(sessions[session_index], &lab_points_reader);
	LabPointsMapReader_GetPointByID(lab_points_reader, player1_position_point_id, &player1_point);
	predicted_new_position_point_id = player1_point.top_connection_id;
	if (predicted_new_position_point_id != 0)
	{
		LabPointsMapReader_GetPointByID(lab_points_reader, predicted_new_position_point_id, &player1_point);
		predicted_new_position_point_id = player1_point.left_connection_id;
	}

	is_player_moved = CommandsListener_Process(instance, &command_player_move);

	if ((is_player_moved == FALSE) && (predicted_new_position_point_id == 0))
	{
		predicted_new_position_point_id = player1_position_point_id;
	}

	Player_GetPositionPointId(player1, &player1_position_point_id);
	assert(player1_position_point_id == predicted_new_position_point_id);
}

int main()
{
	Init();
	Test_CommandPlayerMove();
	Free();
}