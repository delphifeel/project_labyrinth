add_library(GameServer 
	src/lab-points-map.c 
	src/lab-generation.c 
	src/lab-points-map-reader.c 
	src/lab-session.c 
	src/player.c
	src/game-server.c
	src/command.c
	src/commands/command-player-move.c
)

target_include_directories(GameServer PUBLIC "${PROJECT_SOURCE_DIR}/include")