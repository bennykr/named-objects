cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME named_objects)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 17)

set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

add_subdirectory(./googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include) # ${gtest_SOURCE_DIR} - for "gtest_main.cc"

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(run_tests ${TEST_SRC_FILES})
target_link_libraries(run_tests gtest gtest_main)

# add_test(UnitTests run_tests) # for `make test`


# Code coverage:

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
if(CMAKE_COMPILER_IS_GNUCXX)

    # turn off optimizations for accurate code coverage:
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

    include(CodeCoverage)
    append_coverage_compiler_flags()
    set(GCOVR_PATH "/software/x86_64/3.10.0/python/3.7.2/bin/gcovr")

    set(COVERAGE_EXCLUDES "${PROJECT_SOURCE_DIR}/googletest/*")

    setup_target_for_coverage_gcovr_html(NAME run_tests_coverage
                                         EXECUTABLE run_tests
                                         DEPENDENCIES run_tests)

    # target_link_libraries(run_tests gcov) # necessary?
endif()