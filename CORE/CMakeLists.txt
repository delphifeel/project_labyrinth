add_library(CORE 
	src/CORE-system.c
	src/disjoint-set.c
	src/CORE-tcp-server.c
	src/CORE-tcp-client.c
)

target_include_directories(CORE PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/third-party-libraries/libuv/include"
	"${PROJECT_SOURCE_DIR}/third-party-libraries/libuv/src"
)

target_link_libraries(CORE PUBLIC 
	uv
)