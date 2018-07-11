string(
    APPEND opts
    "$<IF:$<CXX_COMPILER_ID:MSVC>,"
        "/W3;$<$<CONFIG:RELEASE>:/O2>,"
				"-Wall;"
            "$<$<CONFIG:RELEASE>:-O3>"
            "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
    ">"
)
add_compile_options("${opts}")

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

link_libraries(ssm)
link_libraries(Catch)

add_executable(vector-test vector.cpp)
add_executable(matrix-test matrix.cpp)
add_executable(quaternion-test quaternion.cpp)
add_executable(transform-test transform.cpp)

add_test(vector vector-test)
add_test(matrix matrix-test)
add_test(quaternion quaternion-test)
add_test(transform transform-test)
