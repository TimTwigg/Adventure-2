cmake_minimum_required(VERSION 3.27.8)
project(Adventure)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# lib
file(GLOB LIB_INC_FILES ${CMAKE_SOURCE_DIR}/lib/*.hpp)
file(GLOB LIB_SRC_FILES ${CMAKE_SOURCE_DIR}/lib/*.cpp)
add_library(mainlib STATIC ${LIB_SRC_FILES} ${LIB_INC_FILES})
target_include_directories(mainlib PUBLIC ${CMAKE_SOURCE_DIR}/lib)
add_executable(Adventure main.cpp)
target_link_libraries(Adventure mainlib)

# tests
enable_testing()
add_subdirectory(tests)