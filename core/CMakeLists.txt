include(libs/cppjieba.cmake)

add_library(SearchCore SHARED src/core.cpp)
target_include_directories(SearchCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(SearchCore PRIVATE cppjieba)
target_compile_features(SearchCore
        PUBLIC
        cxx_nullptr
        PRIVATE
        cxx_lambdas cxx_defaulted_functions)

# add an alias for SearchCore
add_library(HandySearch::SearchCore ALIAS SearchCore)

# enable all warnings
if(MSVC)
    target_compile_options(SearchCore PRIVATE /W4)
else()
    target_compile_options(SearchCore PRIVATE -Wall -Wextra -pedantic)
endif()

if(SEARCH_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

