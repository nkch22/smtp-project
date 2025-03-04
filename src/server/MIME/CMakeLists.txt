set(MIME_SOURCES
    ContentType.cpp
    ContentTransferEncoding.cpp
    Header.cpp
)

set(MIME_HEADERS
    Body.hpp
    ContentTransferEncoding.hpp
    ContentType.hpp
    Header.hpp
    IMimeParser.hpp
    MimeEntity.hpp
    MimeUtils.hpp
)

add_library(mime STATIC ${MIME_SOURCES} ${MIME_HEADERS})
target_include_directories(mime PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(mime PUBLIC encoding)

