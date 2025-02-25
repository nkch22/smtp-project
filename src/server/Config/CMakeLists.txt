add_library(config STATIC
    Config.cpp
)

target_include_directories(config 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/src 
)

target_link_libraries(config
    PUBLIC
    json
)

install(TARGETS config
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES 
    Config.hpp
    DESTINATION include/server/Config
) 
