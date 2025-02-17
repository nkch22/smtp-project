file(GLOB smtp_library_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(smtp_library PRIVATE ${smtp_library_src})
target_include_directories(smtp_library PRIVATE ../)
