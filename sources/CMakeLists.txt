cmake_minimum_required(VERSION 2.9)

project (S8-53M)

set (TARGET_NAME S8-53M)

if(WIN32)
else()
    add_definitions(-fmax-errors=1)
endif()

add_subdirectory(Device)
add_subdirectory(DLoader)
add_subdirectory(Panel)

