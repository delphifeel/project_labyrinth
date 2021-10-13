add_library(CORE 
	src/CORE-system.c
	src/disjoint-set.c
	src/CORE-tcp-server.c
)

target_include_directories(CORE PUBLIC "${PROJECT_SOURCE_DIR}/include")