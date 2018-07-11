add_library(gl-core core.c)
target_include_directories(gl-core INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
