add_library(json STATIC
        JSON.cpp
        Parser.cpp
        Serializer.cpp
        BinarySerializer.cpp
)

target_include_directories(json 
    PUBLIC
        .
)

# Include the tests subdirectory
add_subdirectory(tests)