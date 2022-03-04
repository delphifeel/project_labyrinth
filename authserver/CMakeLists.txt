add_library(AuthServer 
	src/auth-io-system.c
	src/auth-command-types.c
	src/auth-commands-processor.c
	src/authentication/authentication.c
	src/commands/command-authenticate.c
	src/commands/command-join-lobby.c
)
target_include_directories(AuthServer PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(AuthServer PUBLIC 
	CORE
	CommandsProcessor
)