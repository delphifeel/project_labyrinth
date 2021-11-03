add_library(AuthServerTests 
	all-tests.c
	test-auth-commands-processor.c
)
target_include_directories(AuthServerTests PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/lib/commands-processor/include"
)
target_link_libraries(AuthServerTests PUBLIC
	AuthServer
)