cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(fhcrypto VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_package(GTest CONFIG REQUIRED)

# GOST 28147 89
add_executable(test_GOST_28147_89 28147-89-CFB-mode/test.cpp)
target_link_libraries(test_GOST_28147_89 PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_GOST_28147_89.exe test_GOST_28147_89)

add_executable(test_utils utils/test.cpp)
target_link_libraries(test_utils PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(test_utils.exe test_utils)

add_executable(GOST_28147_89 28147-89-CFB-mode/main.cpp)

# STB 34 101 31
add_executable(STB STB-34-101-31/main.cpp
                    )

include_directories(${CMAKE_SOURCE_DIR}/utils/)
