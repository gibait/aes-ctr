cmake_minimum_required(VERSION 3.26)

project(AES-CTR-128-threaded)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIE -pie -g")

include_directories(inc)
include_directories(src)

# Create bin directory if it doesn't exist
file(MAKE_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_library(cipher STATIC src/cipher_operations.cpp)
target_link_directories(cipher PUBLIC inc)
target_link_directories(cipher PRIVATE src)

add_library(aes STATIC src/aes.cpp src/core.cpp inc/utils.h)
target_link_libraries(aes cipher)

add_executable(test test/test.cpp)
target_link_libraries(test aes)