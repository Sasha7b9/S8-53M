cmake_minimum_required(VERSION 2.4)

project (S8-53M)

set (TARGET_NAME S8-53M)

set(CMAKE_UNITY_BUILD ON)

add_subdirectory(Device)
add_subdirectory(DLoader)
add_subdirectory(Panel)

