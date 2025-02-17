file(GLOB smtp_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(smtp PRIVATE ${smtp_src})
add_subdirectory(Protocol)