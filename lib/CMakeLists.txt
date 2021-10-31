add_library(CommandsProcessor 
	commands-processor/src/command.c
	commands-processor/src/commands-processor.c
)

target_include_directories(CommandsProcessor PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	commands-processor/include
)