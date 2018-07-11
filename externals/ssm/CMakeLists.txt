cmake_minimum_required(VERSION 3.5)

project(ssm)

add_library(ssm INTERFACE)
target_include_directories(ssm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_TESTS "Whether or not to build tests" OFF)
if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
