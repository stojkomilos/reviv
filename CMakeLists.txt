cmake_minimum_required(VERSION 3.18)

project(reviv VERSION 1.0)

include(cmake/StandardProjectSettings.cmake)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wpedantic -Wnull-dereference -Wdouble-promotion -Wformat=2 -g")
# -Wshadow -Wunused -Wold-style-cast -Wconversion -Wsign-conversion 

#add_library(project_warnings INTERFACE)
#include(cmake/CompilerWarnings.cmake)
#set_project_warnings(project_warnings)

set(MY_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reviv) # Library before dependency
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)

# PUBLIC -> svi projekti koji ukljuce taj imaju to
# PRIVATE -> samo taj projekat ima to
# INTERFACE -> svi projekti koji ukljuce taj projekat imaju to, ali /sam taj/ projekat /nema/
