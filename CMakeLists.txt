cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME named_objects)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 17)

set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include) # ${gtest_SOURCE_DIR} - for "gtest_main.cc"

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(run_tests ${TEST_SRC_FILES})
target_link_libraries(run_tests gtest gtest_main)

# add_test(UnitTests run_tests) # for `make test`



set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
if(CMAKE_COMPILER_IS_GNUCXX)

    include(CodeCoverage)
    append_coverage_compiler_flags()

    # turn off optimizations for accurate code coverage:
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

    # Note - this is still insufficient, since unused inline functions are
    # removed by the compiler, and do not affect line coverage percentage.
    # compiler options "-fno-inline -fno-inline-small-functions -fno-default-inline"
    # dont seem to work, and "--keep-static-functions --keep-inline-functions"
    # cause linker errors.
    
    set(COVERAGE_EXCLUDES "${PROJECT_SOURCE_DIR}/googletest/*")

    setup_target_for_coverage_gcovr_html(NAME run_tests_coverage
                                         EXECUTABLE run_tests
                                         DEPENDENCIES run_tests)

    # target_link_libraries(run_tests gcov) # necessary?
endif()