# CMake configuration
cmake_minimum_required (VERSION 2.8.12)

enable_language(C)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED)
set (LIB_NAME scds)
set (CMAKE_C_CLANG_TIDY "clang-tidy" "-checks=clang-analyzer-*, concurrency-*, misc-*, performance-*, portability-*, readability-*, -clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling, -misc-unused-parameters, -clang-diagnostic-unused-parameter, -readability-non-const-parameter" "-header-filter=*")
# "-warnings-as-errors=*, -readability-non-const-parameter, -misc-unused-parameters, -clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling"

project (${LIB_NAME} C)

# Build configuration
add_definitions("-Wall -pedantic")

include_directories(${PROJECT_SOURCE_DIR}/include)

# Library configuration
file(GLOB_RECURSE SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(${LIB_NAME} ${SRC_FILES})
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic)
