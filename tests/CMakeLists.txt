find_package(Qt5 COMPONENTS Test REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/core/include)

set(CMAKE_AUTOMOC ON)

set(Test_SRCS testcore.cpp)
qt5_add_resources(Test_QRC tests.qrc)

add_executable(testcore ${Test_SRCS} ${Test_QRC})
add_test(testcore testcore)

target_link_libraries(testcore PUBLIC SearchCore Qt5::Test)
target_compile_options(testcore PRIVATE --coverage)
target_link_libraries(testcore PRIVATE --coverage)