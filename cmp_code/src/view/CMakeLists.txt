cmake_minimum_required(VERSION 3.5.0)
project(view)

add_library(${PROJECT_NAME} STATIC View.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE database)
