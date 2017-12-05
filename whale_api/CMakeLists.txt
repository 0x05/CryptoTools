find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62.0 COMPONENTS filesystem)

file(GLOB SOURCE_FILES
        "*.h"
        "*.cpp"
        "*.hpp"
        )

set(SOURCE_FILES ${SOURCE_FILES})

add_library(whale_api ${SOURCE_FILES})
target_include_directories(whale_api PUBLIC ${JSON})
target_link_libraries(whale_api ${CURL_LIBRARIES})
target_link_libraries(whale_api Boost::filesystem)