file(GLOB SRCS *.cpp)

add_executable(test_tech_analysis ${SRCS})

target_link_libraries(test_tech_analysis tech_analysis gtest gtest_main gmock)

add_test(NAME test_tech_analysis COMMAND test_tech_analysis)
