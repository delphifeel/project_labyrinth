add_library(CORE 
	src/CORE-system.c
	src/disjoint-set.c
)

target_include_directories(CORE PUBLIC "${PROJECT_SOURCE_DIR}/include")