cmake_minimum_required(VERSION 2.8.9)
project(statemachine)

# find_package(Threads REQUIRED)

# Bring the headers
include_directories(include)

# Can manually add the sources using the set command as follows:
set(SOURCES src/statemachine.cpp)

# However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.cpp")

add_executable(statemachine_example ${SOURCES})

# required for forcing -lpthread
# target_link_libraries(state_machine_example ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} -lrt)
