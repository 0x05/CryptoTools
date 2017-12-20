file(GLOB SOURCE_FILES
        "*.h"
        "*.cpp"
        "*.hpp"
        )

set(SOURCE_FILES ${SOURCE_FILES})

add_library(tech_analysis ${SOURCE_FILES})
