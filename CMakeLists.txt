cmake_minimum_required(VERSION 3.18)

project(reviv)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
             "Debug"
             "Release")
endif()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Enable link time optimization if it's Release mode (must be before "add_library" or "add_executable")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "=====> Release mode ${PROJECT_NAME}: enabling LTO")
endif()

# "Add" other cmake projects to current project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reviv) # Library before dependency
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)

if(MSVC) # Set sandbox as the default Visual Studio project
    set(VS_STARTUP_PROJECT "sandbox")
endif()

# PUBLIC -> svi projekti koji ukljuce taj imaju to
# PRIVATE -> samo taj projekat ima to
# INTERFACE -> svi projekti koji ukljuce taj projekat imaju to, ali /sam taj/ projekat /nema/
