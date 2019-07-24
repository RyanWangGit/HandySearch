cmake_minimum_required(VERSION 3.5)

project(HandySearch
        VERSION 2.0
        LANGUAGES CXX)

option(BuildTest "Build the tests." ON)

if(BuildTest)
    enable_testing()
    set(SEARCH_CORE_BUILD_TESTS ON)
endif()

add_subdirectory(core)
add_subdirectory(app)
