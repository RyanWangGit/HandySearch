add_library(cppjieba INTERFACE)
target_include_directories(cppjieba
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cppjieba/deps>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cppjieba/include>)
