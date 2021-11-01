add_library(CORETests 
	all-tests.c
	test-core-tcp-server.c
)
target_include_directories(CORETests PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(CORETests
	CORE
	uv
)