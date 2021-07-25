cmake_minimum_required(VERSION 3.18)

project(sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wpedantic -Wnull-dereference -Wdouble-promotion -Wformat=2 -g")
# -Wshadow -Wunused -Wold-style-cast -Wconversion -Wsign-conversion 
# -E -> stop after preprocessor runs
#set_target_properties(${PROJECT-NAME} PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wpedantic -Wnull-dereference -Wdouble-promotion -Wformat=2 -g")

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Copies the assets folder from sandbox to the directory of the output executable (there might be faster solutions that copy and when the asset files change, and not every time like it does right now)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets
        COMMENT "=====> Copying application assets, from: ${CMAKE_CURRENT_SOURCE_DIR}/assets/ to: ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets")

    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../reviv/src/)

target_link_libraries(${PROJECT_NAME} reviv)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_STARTUP_PROJECT "${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${MY_EXECUTABLE_OUTPUT_DIRECTORY}")
endif()
#install(TARGETS ${PROJECT_NAME} DESTINATION bin) # Should be last command
