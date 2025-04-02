add_library(config STATIC
        Config.cpp
)

target_include_directories(config 
    PUBLIC
        .
    ${CMAKE_SOURCE_DIR}/src 
)

target_link_libraries(config
    PUBLIC
    json
)

