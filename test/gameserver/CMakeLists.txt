add_library(GameServerTests 
	all-tests.c
	test-command.c
	test-commands-processor.c
	test-lab-generation.c
	test-lab-session.c
	#test-player.c
)
target_include_directories(GameServerTests PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/lib/commands-processor/include"
)
target_link_libraries(GameServerTests
	CCORE
	GameServer	
	CommandsProcessor
)
