file(GLOB SRCS *.cpp)

add_executable(test_whale ${SRCS})

target_link_libraries(test_whale whale_api gtest gtest_main gmock)

add_test(NAME test_whale COMMAND test_whale)
