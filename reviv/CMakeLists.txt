cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(reviv VERSION 1.0)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

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
