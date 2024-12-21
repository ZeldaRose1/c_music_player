cmake_minimum_required(VERSION 3.5.0)
project(cmp)

include_directories(${CMAKE_SOURCE_DIR}/libraries)
find_package(PkgConfig REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

pkg_check_modules(TAGLIB REQUIRED taglib)

set(CMAKE_CXX_STANDARD 17)

ADD_LIBRARY(LibsModule
    Database.cpp
    View.cpp
    Config.cpp
)

add_executable(${PROJECT_NAME} cmp.cpp Database.cpp View.cpp Config.cpp)  # Builds in debug by default
add_compile_options(-l sqlite3)
target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC
        sqlite3 
        ${TAGLIB_LIBRARIES}
        ${CURSES_LIBRARIES}
        LibsModule
)

# Link against target using pkg-config
include_directories(${TAGLIB_INCLUDE_DIRS})