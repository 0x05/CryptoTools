#add_subdirectory(${GTEST_ROOT} gtest)
add_subdirectory(${GMOCK_ROOT} gmock)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_subdirectory(test_tech_analysis)
add_subdirectory(test_whale)