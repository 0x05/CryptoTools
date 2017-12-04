find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

file(GLOB SOURCE_FILES
        "*.h"
        "*.cpp"
        "*.hpp"
        )

add_library(whale_api ${SOURCE_FILES})
target_link_libraries(whale_api ${CURL_LIBRARIES})