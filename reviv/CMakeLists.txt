cmake_minimum_required(VERSION 3.18)

project(reviv)

set(GLFW_BUILD_DOCS "OFF")
set(GLFW_BUILD_EXAMPLES "OFF")
set(GLFW_BUILD_TESTS "OFF")

set(BUILD_SHARED_LIBS "OFF")
set(ASSIMP_INSTALL "OFF")
set(ASSIMP_BUILD_TESTS "OFF")

if(DEFINED ENV{WAYLAND_DISPLAY})
    message(STATUS "====> WAYLAND DETECTED, COMPILING GLFW FOR WAYLAND")
    set(GLFW_USE_WAYLAND "ON")
endif()

file(GLOB_RECURSE SRC_FILES "src/*.c"
    "src/*.cpp"
    "vendor/Glad/src/glad.c"
    "vendor/stb_image/stb_image.cpp")

add_library(reviv STATIC ${SRC_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rv_pch.hpp)

# Disabled precompiled headers for the C file. There are workarounds (probably) for .c files
set_source_files_properties("vendor/Glad/src/glad.c" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE GL pthread dl)
#target_link_libraries(${PROJECT_NAME} wayland-client)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp)

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
