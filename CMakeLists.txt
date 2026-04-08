cmake_minimum_required(VERSION 3.10)
project(deque)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
if(SOURCES)
    add_executable(code ${SOURCES})
else()
    # If no cpp files, maybe the OJ compiles it differently, but we need a dummy to avoid cmake error
    add_custom_target(code)
endif()