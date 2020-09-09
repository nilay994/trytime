# Declare the version of the CMake API for forward-compatibility
cmake_minimum_required(VERSION 2.8)

# Declare the name of the CMake Project
project(pacman_orig)

# Add the directory to search for header files
include_directories(include)

# Define an executable target
# Task 1:
# add_executable(init_robots init_robots.cpp RobotClass.cpp)

# Task 2: 
# add_executable(init_robots_with_grid init_robots_with_grid.cpp RobotClass.cpp)

# Task 3:
# add_executable(game_static_robots game_static_robots.cpp RobotClass.cpp)

# Task 4: 
add_executable(pacman_orig pacman_orig.cpp RobotClass.cpp)

add_executable(pacman pacman.cpp)

