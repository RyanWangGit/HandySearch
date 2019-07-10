cmake_minimum_required(VERSION 3.5)

project(HandySearch
        VERSION 2.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# show compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

option(BuildTest "Build the tests." ON)

add_subdirectory(core)
add_subdirectory(app)

if(BuildTest)
    enable_testing()
    add_subdirectory(tests)
endif()
