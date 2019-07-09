find_package(Qt5 COMPONENTS Test REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

include_directories(${PROJECT_SOURCE_DIR}/core/include)

set(CMAKE_AUTOMOC ON)

set(Test_SRCS testcore.cpp)
qt5_add_resources(Test_QRC tests.qrc)

add_executable(testcore ${Test_SRCS} ${Test_QRC})
add_test(testcore testcore)

target_link_libraries(testcore PUBLIC SearchCore Qt5::Test)