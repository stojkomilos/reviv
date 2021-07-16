cmake_minimum_required(VERSION 3.18)
project(reviv VERSION 1.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reviv) # Library before dependency
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

#target_compile_definitions(say-hello PUBLIC SAY_HELLO=1)

# PUBLIC -> svi projekti koji ukljuce taj imaju to
# PRIVATE -> samo taj projekat ima to
# INTERFACE -> svi projekti koji ukljuce taj projekat imaju to, ali /sam taj/ projekat /nema/



