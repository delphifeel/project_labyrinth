cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

add_compile_options(-Wall -pedantic)

add_subdirectory(gameserver)
add_subdirectory(CORE)
add_executable(ProjectLabyrinth gameserver/test/test-lab-session.c)

target_include_directories(ProjectLabyrinth PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(ProjectLabyrinth PUBLIC gameserver CORE)