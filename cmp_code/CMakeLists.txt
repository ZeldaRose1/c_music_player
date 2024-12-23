# Set minimum version of cmake needed to compile project
cmake_minimum_required(VERSION 3.5.0)

# Define project name and version
project(cmp VERSION 1.0.0)

# Enable C++17 or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally specify comipler options
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Add subdirectories for modules
add_subdirectory(src)

# Include third-party libraries
find_package(PkgConfig REQUIRED)
find_package(Curses REQUIRED)

pkg_check_modules(TAGLIB REQUIRED taglib)