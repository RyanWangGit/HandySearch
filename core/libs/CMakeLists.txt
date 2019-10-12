add_library(cppjieba INTERFACE)
set(CPPJIEBA_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/cppjieba/include
        ${CMAKE_CURRENT_SOURCE_DIR}/cppjieba/deps
        )

file(GLOB_RECURSE CPPJIEBA_HEADERS ${CPPJIEBA_SRC} *.hpp)
target_sources(cppjieba INTERFACE ${CPPJIEBA_HEADERS})
target_include_directories(cppjieba INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/cppjieba/include
        ${CMAKE_CURRENT_SOURCE_DIR}/cppjieba/deps)

if(MSVC)
    target_compile_options(cppjieba INTERFACE /W1)
else()
    target_compile_options(cppjieba INTERFACE -w)
endif()