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