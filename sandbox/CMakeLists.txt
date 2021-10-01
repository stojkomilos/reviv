cmake_minimum_required(VERSION 3.18)

project(sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../reviv/src/)

target_link_libraries(${PROJECT_NAME} reviv)

# Handles the asset files. On release it just copies them over. On debug it creates a symlink on Linux, and on Windows it changes the Visual Studio working directory to the source assets. (copies just like in release if it's Debug but neither Linux nor Visual Studio)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Copies the assets folder from sandbox to the directory of the output executable (there might be faster solutions that copy and when the asset files change, and not every time like it does right now)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets
        COMMENT "=====> Release: Copying application assets, from: ${CMAKE_CURRENT_SOURCE_DIR}/assets/ to: ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets"
    )
else()
    if(MSVC)
        # Sets Visual Studio working directory to the directory of the executable for accessing the asset files
        set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
    elseif(UNIX AND NOT APPLE) # Creates a symlink on Linux for the asset files
        set(MY_ASSETS_LINK_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets")
        set(MY_ASSETS_LINK_DESTINATION "${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets")
        add_custom_command(
             TARGET ${PROJECT_NAME} POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E create_symlink ${MY_ASSETS_LINK_SOURCE} ${MY_ASSETS_LINK_DESTINATION}
             DEPENDS ${MY_ASSETS_LINK_SOURCE}
             COMMENT "symbolic link resources folder from ${MY_ASSETS_LINK_SOURCE} => ${MY_ASSETS_LINK_DESTINATION}"
        )
    else()
        message(MESSAGE "======> ERROR (Debug build): not building on neither MSVC (Microsoft Windows) nor on Linux. Don't know how to handle linking asset files. I am just going to copy the asset files.")
        # Copies the assets folder from sandbox to the directory of the output executable (there might be faster solutions that copy and when the asset files change, and not every time like it does right now)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets/
            ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets
            COMMENT "=====> Debug: Copying application assets, from: ${CMAKE_CURRENT_SOURCE_DIR}/assets/ to: ${MY_EXECUTABLE_OUTPUT_DIRECTORY}/assets"
        )
    endif()
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4" "/MP" "/std:c++latest")
else()	
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-g" "-Wextra" "-Wnon-virtual-dtor" "-Woverloaded-virtual" "-Wno-unused-variable" "-Wno-unused-parameter" "-Wnull-dereference" "-Wformat=2")
    #set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall -g -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wno-unused-variable -Wno-unused-parameter -Wnull-dereference -Wformat=2")
endif()
