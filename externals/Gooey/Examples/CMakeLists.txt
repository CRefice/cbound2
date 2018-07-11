add_gooey_executable(ControlsExample Controls.cpp)
target_link_libraries(ControlsExample Gooey)
target_include_directories(ControlsExample PRIVATE "${PROJECT_SOURCE_DIR}/Include")

set(GOOEY_EXAMPLES ControlsExample)

install(TARGETS ${GOOEY_EXAMPLES} RUNTIME DESTINATION "Bin/Examples")
