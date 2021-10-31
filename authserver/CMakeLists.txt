add_library(AuthServer 
	src/auth-io-system.c
	src/auth-command-types.c

	src/commands/command-authenticate.c
)

target_include_directories(AuthServer PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/lib/commands-processor/include"
)