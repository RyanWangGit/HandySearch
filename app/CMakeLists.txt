find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(HandySearch ${SOURCES} app.qrc)
target_include_directories(HandySearch PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(HandySearch PRIVATE SearchCore Qt5::Widgets Qt5::Gui)

# enable all warnings
if(MSVC)
    target_compile_options(HandySearch PRIVATE /W4)
else()
    target_compile_options(HandySearch PRIVATE -Wall -Wextra -pedantic)
endif()
