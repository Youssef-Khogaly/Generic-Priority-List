cmake_minimum_required(VERSION 3.10)

project(list_example)

set(HEADER_FILES list.h)
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} list_example.c list.c)

set(COMPILER_FLAGS   -w -Wall -Wextra -O3)

add_compile_options(${COMPILER_FLAGS})

add_executable(list_example  ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(list_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(list_example PROPERTIES LINKER_LANGUAGE C)