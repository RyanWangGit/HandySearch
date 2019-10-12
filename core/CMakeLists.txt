cmake_minimum_required(VERSION 3.5)

find_package(Qt5 COMPONENTS Core Sql Concurrent REQUIRED)

add_subdirectory(libs)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${INCLUDE_DIR})

set(SearchCore_SRCS ${SOURCE_DIR}/searchcore.cpp)

# manually wrap header file since AUTOMOC doesn't seem to handle files in different folder
qt5_wrap_cpp(SearchCore_INCLUDE ${INCLUDE_DIR}/searchcore.h)

add_library(SearchCore SHARED ${SearchCore_INCLUDE} ${SearchCore_SRCS} core.qrc)

target_link_libraries(SearchCore PUBLIC cppjieba Qt5::Core Qt5::Concurrent Qt5::Sql)
target_include_directories(SearchCore PUBLIC ${INCLUDE_DIR})
