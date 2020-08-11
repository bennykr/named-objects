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
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
#if(CMAKE_COMPILER_IS_GNUCXX)
#    include(CodeCoverage)
#    setup_target_for_coverage(${PROJECT_NAME}_coverage run_tests coverage)
#endif()
#if(CMAKE_COMPILER_IS_GNUCXX)
#    target_link_libraries(run_tests gcov)
#endif()