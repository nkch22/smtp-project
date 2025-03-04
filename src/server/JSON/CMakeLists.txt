add_library(json STATIC
    JSON.cpp
    Parser.cpp
    Serializer.cpp
)

target_include_directories(json 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

