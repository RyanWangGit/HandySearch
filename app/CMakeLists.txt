find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HandySearch_SRCS
        ${SOURCE_DIR}/handy_search.cpp
        ${SOURCE_DIR}/load_ui.cpp
        main.cpp)

add_executable(HandySearch ${HandySearch_SRCS} ${HandySearch_UIS} app.qrc)

if(MSVC)
    target_compile_options(SearchCore PRIVATE /W4)
else()
    target_compile_options(SearchCore PRIVATE -Wall -Wextra -pedantic)
endif()

target_link_libraries(HandySearch PUBLIC SearchCore Qt5::Widgets Qt5::Gui)