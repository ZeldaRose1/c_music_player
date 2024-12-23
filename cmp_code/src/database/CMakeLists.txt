project(database)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)

add_library(${PROJECT_NAME} STATIC Database.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${TAGLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE view ${TAGLIB_LIBRARIES})

include_directories(${TAGLIB_INCLUDE_DIRS})


# Print folders for taglib to verify cmake can find them proprerly
# message(STATUS "TAGLIB_INCLUDE_DIRS: ${TAGLIB_INCLUDE_DIRS}")
# message(STATUS "TAGLIB_LIBRARIES: ${TAGLIB_LIBRARIES}")