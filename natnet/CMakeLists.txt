cmake_minimum_required(VERSION 3.1.0)
project(natnet)

# Bring the headers
include_directories(include/arch/linux)

add_library(LibsModule
            include/arch/linux/udp_socket.c)

target_link_libraries(LibsModule -lpthread)

# Can manually add the sources using the set command as follows:
set(SOURCES natnet.c)

# However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "./*.c")

add_executable(natnet_example ${SOURCES})

# required for forcing -lpthread
# target_link_libraries(state_machine_example ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} -lrt)
target_link_libraries(natnet_example LibsModule)