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

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../reviv/src/)

target_link_libraries(${PROJECT_NAME} reviv)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Copies the assets folder from sandbox to the directory of the output executable (there might be faster solutions that copy and when the asset files change, and not every time like it does right now)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets
        COMMENT "=====> Release: Copying application assets, from: ${CMAKE_CURRENT_SOURCE_DIR}/assets/ to: ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets"
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #if(MSVC)
    #    # Sets Visual Studio working directory to the directory of the executable
    #    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
    #elseif(UNIX AND NOT APPLE)
    #    ADD_CUSTOM_TARGET(${PROJECT_NAME} ALL
    #                      COMMAND ${CMAKE_COMMAND} -E create_symlink ${target} ${link})
    #else()
    #    message(FATAL_ERROR "ERROR (Debug build): not building on neither MSVC (Microsoft Windows) nor on Linux. Don't know how to handle linking asset files")
    set(MY_ASSETS_LINK_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set(MY_ASSETS_LINK_DESTINATION "${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets")
    add_custom_command(
         TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E create_symlink ${MY_ASSETS_LINK_SOURCE} ${MY_ASSETS_LINK_DESTINATION}
         DEPENDS ${MY_ASSETS_LINK_SOURCE}
         COMMENT "symbolic link resources folder from ${MY_ASSETS_LINK_SOURCE} => ${MY_ASSETS_LINK_DESTINATION}"
    )
    #endif()

else()
    message(SEND_ERROR "Build type is is not recognized. ('CMAKE_BUILD_TYPE' neither 'Release' nor 'Debug'.")
endif()


#install(TARGETS ${PROJECT_NAME} DESTINATION bin) # Should be last command
