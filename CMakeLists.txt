cmake_minimum_required(VERSION 3.7)
project(crypto_tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(whale_api)
add_subdirectory(test)
add_subdirectory(main)
add_subdirectory(CryptoTools)