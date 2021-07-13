# good resource - https://codevion.github.io/#!cpp/cmake.md
# bash: "cmake --help-command <any cmake command, e.g. link_directories>

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(reviv VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

# -Werror -> treat warnings as errors
# -Wall

file(GLOB_RECURSE SRC_FILES "src/*.c"
    "src/*.cpp"
    "vendor/Glad/src/glad.c"
    "vendor/stb_image/stb_image.cpp")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/src)

target_link_libraries(${PROJECT_NAME} GL pthread dl)
#target_link_libraries(${PROJECT_NAME} wayland-client)
target_link_libraries(${PROJECT_NAME} glfw)

add_compile_definitions(RV_DEBUG=1)
add_compile_definitions(RV_RELEASE=0)

# Archive: ===
#   link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib)
#   find_library(glfw3_library ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/libglfw3.a REQUIRED)
#   find_library(GL_library GL REQUIRED)
#   find_library(wayland-client_library wayland-client REQUIRED)
#   find_library(pthread_library pthread REQUIRED)
#   find_library(dl_library dl REQUIRED)
#   target_link_libraries(${PROJECT_NAME} PUBLIC glfw3 ${GL_library} ${wayland-client_library} ${pthread_library} ${dl_library})
