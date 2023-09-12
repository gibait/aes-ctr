cmake_minimum_required(VERSION 3.26)

project(AES-CTR-128-threaded)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)
include_directories(src)

add_library(cipher STATIC src/cipher_operations.cpp)
target_link_directories(cipher PUBLIC inc)
target_link_directories(cipher PRIVATE src)

add_library(aes STATIC src/aes.cpp src/core.cpp inc/utils.h)
target_link_libraries(aes cipher)

add_executable(test test/test_api.cpp)
target_link_libraries(test aes)