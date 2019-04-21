# compile Catch library with the main function to speed up the compilation
add_library(catch_main OBJECT test.cpp)
target_link_libraries(catch_main PRIVATE catch)

file(GLOB files "test_*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "test_([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})

    target_include_directories(${testcase} PRIVATE ${INCLUDE_DIR})
    target_link_libraries(${testcase} PRIVATE ${HANDYSEARCH_TARGET} catch)

    add_test(NAME ${testcase}
            COMMAND ${testcase}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
