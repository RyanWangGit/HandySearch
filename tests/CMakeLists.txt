add_subdirectory(libs/Catch2)

# compile Catch library with the main function to speed up the compilation
add_library(catch_main OBJECT test.cpp)
target_link_libraries(catch_main PRIVATE Catch2)

file(GLOB files "test_*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "test_([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})

    target_link_libraries(${testcase} PRIVATE Catch2 SearchCore)

    add_test(NAME ${testcase}
            COMMAND ${testcase}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
