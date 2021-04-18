CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

PROJECT(LinuxKeyLogger VERSION 0.1)

add_executable(logger main.c kbdEvents.c)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})