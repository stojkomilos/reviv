cmake_minimum_required(VERSION 3.18)

project(reviv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Glfw settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Glfw: build docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Glfw: build examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Glfw: build tests" FORCE)

# Assimp settings
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shard libs" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp: build install" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp: build tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Assimp: Build command line tool, (Probably, idk)" FORCE)

# Compile GLFW for wayland if it's detected
if(DEFINED ENV{WAYLAND_DISPLAY})
    message(STATUS "====> WAYLAND DETECTED, COMPILING GLFW FOR WAYLAND")
    set(GLFW_USE_WAYLAND "ON")
endif()

# Add all the translation units to the project
file(GLOB_RECURSE SRC_FILES_ALL "src/*.cpp"
    "vendor/Glad/src/glad.c"
    "vendor/stb_image/stb_image.cpp")

file(GLOB_RECURSE SRC_FILES_DEPENDENCIES
    "vendor/Glad/src/glad.c"
    "vendor/stb_image/stb_image.cpp")
set_source_files_properties(${SRC_FILES_DEPENDENCIES} PROPERTIES COMPILE_FLAGS -Wno-all -g -Wno-extra -Wno-overloaded-virtual -Wno-unused-variable -Wno-unused-parameter -Wno-null-dereference -Wno-format=2)


#set_source_files_properties(SRC_FILES_DEPENDENCIES PROPERTIES COMPILE_FLAGS -Wno-Wall -Wno-Wextra -Wno-Wdouble-promotion )

# When compiling this project, compile it as a static library
add_library(reviv STATIC ${SRC_FILES_ALL})

#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wnull-dereference -Wdouble-promotion -Wformat=2 -g")

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall -g -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wno-unused-variable -Wno-unused-parameter -Wnull-dereference -Wformat=2")
# -Wshadow -Wunused -Wold-style-cast -Wconversion -Wsign-conversion 
# -E -> stop after preprocessor runs

# Add other cmake projects to this one
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp)

# Add a precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rv_pch.hpp)

# Disabled precompiled headers for the C file. There are workarounds (probably) for .c files
set_source_files_properties("vendor/Glad/src/glad.c" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# The include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp_config_h) # Stupid assimp doing things it's own way (https://stackoverflow.com/questions/60261675/assimp-config-h-not-found-its-in-the-folder-though)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE GL pthread dl)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp)

# Set defines depending on wheather this is a debug or release build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC RV_DEBUG=1)
    message(STATUS "========> Debug mode: Enabling Debug compiler definition (RV_DEBUG=1)")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PUBLIC RV_DEBUG=0)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NDEBUG=1)
    message(STATUS "========> Release mode: Disabling Debug compiler definition (RV_DEBUG=0)")
    message(STATUS "========> Release mode: Defining NDEBUG")
else()
    message(FATAL_ERROR "========> ERROR: CMAKE_BUILD_TYPE is not recognized. Should be either \"Release\" or \"Debug\"!!!")
endif()

#target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rv_pch.hpp)
