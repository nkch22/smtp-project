add_library(json STATIC
    JSON.cpp
    Parser.cpp
    Serializer.cpp
)

target_include_directories(json 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

install(TARGETS json
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES 
    JSON.hpp
    Parser.hpp
    Serializer.hpp
    JSONAll.hpp
    Macros.hpp
    PreprocessorUtilities.hpp
    SerializerVectorImpl.hpp
    DESTINATION include/server/JSON
) 
