add_library(IOSystem 
	iosystem/iosystem.c
)
target_include_directories(IOSystem PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)
target_link_libraries(IOSystem PRIVATE
	TCP
	CCORE
)