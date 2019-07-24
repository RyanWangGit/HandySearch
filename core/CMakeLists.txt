add_library(SearchCore SHARED src/core.cpp)

target_include_directories(SearchCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(MSVC)
    target_compile_options(SearchCore PRIVATE /W4)
else()
    target_compile_options(SearchCore PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_features(SearchCore
        PUBLIC
        cxx_nullptr
        PRIVATE
        cxx_lambdas cxx_defaulted_functions)