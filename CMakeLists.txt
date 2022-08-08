cmake_minimum_required(VERSION 3.23)
project(Nextcloud_API_Grabber C)

set(CMAKE_C_STANDARD 99)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE INCLUDES "*inc/*.h")
file(GLOB_RECURSE SOURCES "src/*.c")

add_library(Nextcloud_API_Grabber ${SOURCES} ${INCLUDES})

add_subdirectory(examples)