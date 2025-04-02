set(MIME_SOURCES
        ContentType.cpp
        ContentTransferEncoding.cpp
        Header.cpp
        MimeUtils.cpp
        Body.cpp
        MimeEntity.cpp
        MimeBuilder.cpp
        MimeParser.cpp
)

set(MIME_HEADERS
        Body.hpp
        ContentTransferEncoding.hpp
        ContentType.hpp
        Header.hpp
        MimeEntity.hpp
        MimeUtils.hpp
        MimeBuilder.hpp
        MimeParser.hpp
)

add_library(mime STATIC ${MIME_SOURCES} ${MIME_HEADERS})
target_include_directories(mime PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(mime PUBLIC encoding)


