cmake_minimum_required(VERSION 3.5.0)

project(control)

add_library(${PROJECT_NAME} Control.cpp)

## It looks like the files here are unneeded. The libraries need to be linked in the directory
## above this folder, not here.


# find_package(sdl2 REQUIRED)
# find_package(PkgConfig REQUIRED)

# pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
# message("SDL2_MIXER_LIBRARIES: ${SDL2_MIXER_LIBRARIES}")
# message("SDL2_MIXER_LIBRARIES: ${SDL2_MIXER_INCLUDE_DIRS}")


# add_library(${PROJECT_NAME} STATIC Control.cpp)
# target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} PRIVATE database ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})