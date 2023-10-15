cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(fhcrypto VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)

# GOST 28147 89
add_executable(test_GOST_28147_89 28147-89-CFB-mode/test.cpp)
target_link_libraries(test_GOST_28147_89 PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_GOST_28147_89.exe test_GOST_28147_89)

add_executable(test_utils utils/test.cpp)
target_link_libraries(test_utils PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_utils.exe test_utils)

add_executable(GOST_28147_89 28147-89-CFB-mode/main.cpp)

# STB 34 101 31
add_executable(test_STB STB-34-101-31/test.cpp)
target_link_libraries(test_STB PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_STB.exe test_STB)

add_executable(STB STB-34-101-31/main.cpp)

# rabin
add_executable(rabin rabin/main.cpp)

target_link_libraries(rabin PkgConfig::gmp)
target_link_libraries(rabin PkgConfig::gmpxx)

# Test
add_executable(Test11 Lab1/Test1/main.cpp)
add_executable(Test11_test Lab1/Test1/test.cpp)
target_link_libraries(Test11_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(Test11.exe Test11_test)

add_executable(Test12 Lab1/Test2/main.cpp)
add_executable(Test12_test Lab1/Test2/test.cpp)
target_link_libraries(Test12_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(Test12.exe Test12_test)

add_executable(Test13 Lab1/Test3/main.cpp)
add_executable(Test13_test Lab1/Test3/test.cpp)
target_link_libraries(Test13_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(Test13.exe Test13_test)

add_executable(Test14 Lab1/Test4/main.cpp)
add_executable(Test14_test Lab1/Test4/test.cpp)
target_link_libraries(Test14_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(Test14.exe Test14_test)

add_executable(Test15 Lab1/Test5/main.cpp)
add_executable(Test15_test Lab1/Test5/test.cpp)
target_link_libraries(Test15_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(Test15.exe Test15_test)

# add_executable(Test16 Lab1/Test6/main.cpp)
# add_executable(Test16_test Lab1/Test6/test.cpp)
# target_link_libraries(Test16_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
# add_test(Test16.exe Test16_test)

include_directories(${CMAKE_SOURCE_DIR}/utils/)
