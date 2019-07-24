add_library(SearchCore SHARED src/core.cpp)

target_include_directories(SearchCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(SearchCore
        PUBLIC
        cxx_nullptr
        PRIVATE
        cxx_lambdas cxx_defaulted_functions)