cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(GOST_31_11 main.cpp)

add_executable(test_GOST_31_11 test.cpp)
target_link_libraries(test_GOST_31_11 PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_GOST_31_11.exe test_GOST_31_11)

target_link_libraries(GOST_31_11 PkgConfig::gmp)
target_link_libraries(GOST_31_11 PkgConfig::gmpxx)
