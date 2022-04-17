add_library(GameServer 
	src/room-info.c 
	src/lab-points-map.c 
	src/lab-generation.c 
	src/lab-session.c 
	src/player.c
	src/game-server.c
	src/gameserver-command.c
	src/gameserver-command-response.c
	src/gameserver-command-types.c
	src/gameserver-commands-processor.c
	src/commands-io-system.c

	src/commands/command-player-move.c
	src/commands/command-start-game.c
	src/commands/command-player-init.c
)

target_include_directories(GameServer PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(GameServer PUBLIC 
	CCORE
	CommandsProcessor
	TCP
)