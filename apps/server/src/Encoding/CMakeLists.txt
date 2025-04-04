add_library(encoding STATIC
        Base64Encoder.cpp
        QuotedPrintableEncoder.cpp
        EncoderFactory.cpp
)

target_include_directories(encoding
    PUBLIC
        .
)

