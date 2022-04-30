cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# main lib
project(mainlib)

file(GLOB LIB_SRC_FILES ${CMAKE_SOURCE_DIR}/lib/*.cpp)
file(GLOB LIB_INC_FILES ${CMAKE_SOURCE_DIR}/lib/*.hpp)
add_library(${PROJECT_NAME} STATIC ${LIB_SRC_FILES} ${LIB_INC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
set(LIB_LIBS ${PROJECT_NAME})


# main.cpp
project(Adventure)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${LIB_LIBS})


# test framework
project(Test)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB GTEST_SRC_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(${PROJECT_NAME} ${GTEST_SRC_FILES})
target_link_libraries(${PROJECT_NAME} gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${LIB_LIBS})