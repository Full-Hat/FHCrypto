cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

find_package(Boost 1.83.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(genkey genpkey.cpp)
add_executable(conv convpkey.cpp)
add_executable(sign sign.cpp)
add_executable(verify verify.cpp)

target_link_libraries(genkey ${Boost_LIBRARIES})
target_link_libraries(conv ${Boost_LIBRARIES})
target_link_libraries(sign ${Boost_LIBRARIES})
target_link_libraries(verify ${Boost_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Hash/bin)

add_custom_command(
    TARGET genkey
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:GOST_31_11>
        ${CMAKE_CURRENT_BINARY_DIR}/Hash/bin
    DEPENDS GOST_31_11
)
