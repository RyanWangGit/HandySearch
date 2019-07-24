set(CMAKE_CXX_STANDARD 14)

option(BuildTest "Build the tests." ON)

add_library(SearchCore SHARED src/core.cpp)

target_include_directories(SearchCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
