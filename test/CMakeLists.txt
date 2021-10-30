add_library(GameServerTest 
	gameserver/test-disjoint-set.c
	gameserver/test-lab-generation.c
	gameserver/test-lab-session.c
	gameserver/test-command.c
	gameserver/test-commands-processor.c
)

target_include_directories(GameServerTest PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(GameServerTest PUBLIC 
	GameServer 
	CORE
)

add_library(AuthServerTest
	authserver/test-message.c
)

target_include_directories(AuthServerTest PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(AuthServerTest PUBLIC 
	AuthServer 
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