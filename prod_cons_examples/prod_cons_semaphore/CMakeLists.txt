cmake_minimum_required(VERSION 3.1.0)
project(prod_cons_semaphore)

set(CMAKE_CXX_STANDARD 11)

# send -g flag
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(prod_cons_semaphore ${SOURCES})

# required for forcing -lpthread
target_link_libraries(prod_cons_semaphore ${CMAKE_THREAD_LIBS_INIT})
