cmake_minimum_required(VERSION 3.18)

#-
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/milos/dev/reviv2/build/bin/${CMAKE_SYSTEM_NAME}-default")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "/home/milos/dev/reviv2/build/bin/${CMAKE_SYSTEM_NAME}-default")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/home/milos/dev/reviv2/build/bin/${CMAKE_SYSTEM_NAME}-default")

set(MY_BINARY_OUTPUT_DIRECTORY "/home/milos/dev/reviv2/build/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${MY_BINARY_OUTPUT_DIRECTORY}/debug-${CMAKE_SYSTEM_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${MY_BINARY_OUTPUT_DIRECTORY}/debug-${CMAKE_SYSTEM_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${MY_BINARY_OUTPUT_DIRECTORY}/debug-${CMAKE_SYSTEM_NAME}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MY_BINARY_OUTPUT_DIRECTORY}/release-${CMAKE_SYSTEM_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MY_BINARY_OUTPUT_DIRECTORY}/release-${CMAKE_SYSTEM_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MY_BINARY_OUTPUT_DIRECTORY}/release-${CMAKE_SYSTEM_NAME}")
#-

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(reviv VERSION 1.0)

set(GLFW_BUILD_DOCS "OFF")
set(GLFW_BUILD_EXAMPLES "OFF")
set(GLFW_BUILD_TESTS "OFF")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

# -Werror -> treat warnings as errors
# -Wall

file(GLOB_RECURSE SRC_FILES "src/*.c"
    "src/*.cpp"
    "vendor/Glad/src/glad.c"
    "vendor/stb_image/stb_image.cpp")

add_library(reviv STATIC ${SRC_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE GL pthread dl)
#target_link_libraries(${PROJECT_NAME} wayland-client)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_compile_definitions(RV_DEBUG=1)
add_compile_definitions(RV_RELEASE=0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
