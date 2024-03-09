cmake_minimum_required(VERSION 2.8.9)
project(alphapilot_example)

# send -g flag
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

# Bring the headers
include_directories(include)

# Can manually add the sources using the set command as follows:
set(SOURCES src/alphapilot_example.cpp)

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(alphapilot_example ${SOURCES})

# required for forcing -lpthread
target_link_libraries(spsc_example ${CMAKE_THREAD_LIBS_INIT} )
