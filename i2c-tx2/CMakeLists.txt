cmake_minimum_required(VERSION 2.8.9)
project(i2c_example)

# find_package(Threads REQUIRED)

# Bring the headers
include_directories(include)
# include_directories(${Boost_INCLUDE_DIR})

# Can manually add the sources using the set command as follows:
set(SOURCES src/i2c_driver.cpp src/i2c_tfmini.cpp)

# However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.cpp")

add_executable(i2c_example ${SOURCES})

# required for forcing -lpthread
target_link_libraries(i2c_example -lrt)
