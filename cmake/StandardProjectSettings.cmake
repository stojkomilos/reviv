#
#option(ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
#
#if(ENABLE_IPO)
#  include(CheckIPOSupported)
#  check_ipo_supported(
#    RESULT
#    result
#    OUTPUT
#    output)
#  if(result)
#    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
#  else()
#    message(SEND_ERROR "IPO is not supported: ${output}")
#  endif()
#endif()
#if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
#  add_compile_options(-fcolor-diagnostics)
#elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#  add_compile_options(-fdiagnostics-color=always)
#else()
#  message(STATUS "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
#endif()
