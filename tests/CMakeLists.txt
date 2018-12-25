# compile Catch library with the main function to speed up the compilation
add_library(catch_main OBJECT test.cpp)
target_include_directories(catch_main PRIVATE third_party)

file(GLOB files "test_*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "test_([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} ${file})

    target_include_directories(${testcase} PRIVATE ${INCLUDE_DIR} third_party)
    target_link_libraries(${testcase} ${HANDYSEARCH_LIB} catch_main)

    add_test(NAME ${testcase}
            COMMAND ${testcase}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
endforeach()
