file(GLOB GTEST_SRC_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(Tests ${GTEST_SRC_FILES})
target_link_libraries(Tests mainlib gtest_main)
add_test(Tests Tests)