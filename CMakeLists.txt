cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

set(CMAKE_BINARY_DIR  ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(lib)
add_executable(ProjectLabyrinth test/test-player.c)

target_link_libraries(ProjectLabyrinth PUBLIC src lib)
target_include_directories(ProjectLabyrinth PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include/labyrinth")

install(TARGETS ProjectLabyrinth DESTINATION bin)

add_custom_target(run ALL COMMAND ProjectLabyrinth)

enable_testing()

add_test(NAME Runs COMMAND ProjectLabyrinth)


