set(COMMON_INCLUDE 
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)


# IOSystem
add_library(IOSystem 
	iosystem/iosystem.c
)
target_include_directories(IOSystem PUBLIC
	${COMMON_INCLUDE}
)
