project(cmp)

include_directories(${CMAKE_SOURCE_DIR}/libraries)
find_package(PkgConfig REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

pkg_check_modules(TAGLIB REQUIRED taglib)

add_executable(${PROJECT_NAME} cmp.cpp database/Database.cpp view/View.cpp config/Config.cpp)  # Builds in debug by default
add_compile_options(-l sqlite3)
target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC
        sqlite3 
        ${TAGLIB_LIBRARIES}
        ${CURSES_LIBRARIES}
)

add_subdirectory(config)
add_subdirectory(database)
add_subdirectory(view)

# Link against target using pkg-config
include_directories(${TAGLIB_INCLUDE_DIRS})
