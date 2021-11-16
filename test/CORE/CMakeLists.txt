add_library(CORETests 
	all-tests.c
	test-core-tcp-server.c
	test-core-tcp-client.c
	test-core-object.c
	test-core-object-private.c
)
target_include_directories(CORETests PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(CORETests
	CORE
)