cmake_minimum_required(VERSION 3.1.0)
project(prod_cons_ringbuffer)

set(CMAKE_CXX_STANDARD 11)

# send -g flag
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

# Bring the headers
include_directories(include)

# # add utils libs like Ringbuffer
# add_library(RINGBUFFER "src/RingBuffer.cpp" "include/RingBuffer.h")

# Can manually add the sources using the set command as follows:
# set(SOURCES "src/RingBuffer.cpp" "src/consumer.cpp" "src/producer.cpp")

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(prod_cons_ringbuffer ${SOURCES})

# required for forcing -lpthread
target_link_libraries(prod_cons_ringbuffer ${CMAKE_THREAD_LIBS_INIT})
