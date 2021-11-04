add_library(CommandsProcessor 
	commands-processor/src/command.c
)

target_include_directories(CommandsProcessor PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	commands-processor/include
)