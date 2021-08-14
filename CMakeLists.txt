cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth VERSION 0.001)

add_subdirectory(src)
add_subdirectory(lib)
add_executable(ProjectLabyrinth test/lab-prim-algo.c)

target_link_libraries(ProjectLabyrinth PUBLIC src lib)
target_include_directories(ProjectLabyrinth PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/lib")

add_custom_target(run ALL COMMAND ProjectLabyrinth)

enable_testing()

add_test(NAME Runs COMMAND ProjectLabyrinth)


