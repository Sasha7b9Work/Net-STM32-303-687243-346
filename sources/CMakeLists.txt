cmake_minimum_required(VERSION 4.0)

project (STM32-303-687243-346)

set (TARGET_NAME )

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(MultiSensors)
