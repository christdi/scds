# CMake configuration
cmake_minimum_required (VERSION 2.8.12)

enable_language(C)

option(BUILD_TESTS "Build unit tests" ON)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED)
set (LIB_NAME scds)
set (CMAKE_C_CLANG_TIDY "clang-tidy" "-checks=clang-analyzer-*, concurrency-*, misc-*, performance-*, portability-*, readability-*, -clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling, -misc-unused-parameters, -clang-diagnostic-unused-parameter, -readability-non-const-parameter" "-header-filter=*")

project (${LIB_NAME} C)

# Build configuration
add_definitions("-Wall -pedantic")

# Library configuration
file(GLOB_RECURSE SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(${LIB_NAME} ${SRC_FILES})
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic)

if(BUILD_TESTS)

include(CTest)

# Unity testing configuration
add_library(unity ${PROJECT_SOURCE_DIR}/ext/unity/src/unity.c)
target_include_directories(unity PUBLIC ${PROJECT_SOURCE_DIR}/ext/unity/src)
set_target_properties(unity PROPERTIES C_CLANG_TIDY "")

# Testing configuration
file(GLOB_RECURSE TEST_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test/*.c)

add_executable(test_app ${TEST_FILES})
set_target_properties(test_app PROPERTIES C_CLANG_TIDY "")
target_link_libraries(test_app ${LIB_NAME} unity)
add_test(test_suite test_app)

endif()