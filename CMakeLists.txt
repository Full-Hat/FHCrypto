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

# GOST 34 11
add_subdirectory(31-11)

# MD5
add_executable(MD5 MD5/main.cpp MD5/md5.cpp)

find_package(cryptopp CONFIG REQUIRED)
target_link_libraries(MD5 PRIVATE cryptopp::cryptopp)

# GOST 34 10
add_subdirectory(34-10)

include_directories(${CMAKE_SOURCE_DIR}/utils/)
