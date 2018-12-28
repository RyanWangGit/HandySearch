# compile Catch library with the main function to speed up the compilation
add_library(catch_main OBJECT test.cpp)
target_include_directories(catch_main PRIVATE third_party)

file(GLOB files "test_*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "test_([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})

    target_include_directories(${testcase} PRIVATE ${INCLUDE_DIR} third_party)
    target_link_libraries(${testcase} ${HANDYSEARCH_TARGET})

    add_test(NAME ${testcase}
            COMMAND ${testcase}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
endforeach()

# copy the sample dictionary file to binary directory
add_custom_command(
        TARGET test-dictionary POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/sample_dictionary.txt
        ${CMAKE_CURRENT_BINARY_DIR}/sample_dictionary.txt)
