file(GLOB smtp_library_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(smtp PRIVATE ${smtp_library_src})