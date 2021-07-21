cmake_minimum_required(VERSION 3.18)

project(reviv)

include(cmake/StandardProjectSettings.cmake)
#add_library(project_options INTERFACE)
#target_compile_features(project_options INTERFACE cxx_std_17)


#add_library(project_warnings INTERFACE)
#include(cmake/CompilerWarnings.cmake)
#set_project_warnings(project_warnings)

# --- Set output directory
set(MY_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE   "${MY_EXECUTABLE_OUTPUT_DIRECTORY}" CACHE PATH "Directory of the output executable" FORCE)
# ---<

# "Add" other cmake projects to current project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reviv) # Library before dependency
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)

# PUBLIC -> svi projekti koji ukljuce taj imaju to
# PRIVATE -> samo taj projekat ima to
# INTERFACE -> svi projekti koji ukljuce taj projekat imaju to, ali /sam taj/ projekat /nema/
