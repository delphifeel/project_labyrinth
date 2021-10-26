add_library(GameServerTest 
	gameserver/test-disjoint-set.c
	gameserver/test-lab-generation.c
	gameserver/test-lab-session.c
	gameserver/test-command.c
)
target_include_directories(GameServerTest PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(GameServerTest PUBLIC 
	GameServer 
	CORE
)

add_library(CORETest 
	CORE/test-core-tcp-server.c
)
target_include_directories(CORETest PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(CORETest PUBLIC 
	CORE
)