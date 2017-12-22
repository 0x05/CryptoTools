set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.63.0 COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCE_FILES
        "*.h"
        "*.cpp"
        "*.hpp"
        )

set(SOURCE_FILES ${SOURCE_FILES})

add_library(tech_analysis ${SOURCE_FILES})
target_link_libraries(tech_analysis ${Boost_LIBRARIES})