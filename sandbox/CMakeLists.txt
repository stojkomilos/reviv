project(sandbox)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/assets ${MY_EXECUTABLE_OUTPUT_DIRECTORY}
    COMMENT "====> Copying assets folder to the directory of the executable"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../reviv/src/)

target_link_libraries(${PROJECT_NAME} reviv)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin) # Should be last command
