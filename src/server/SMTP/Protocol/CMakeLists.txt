file(GLOB smtp_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(smtp PRIVATE ${smtp_src})
target_include_directories(smtp PUBLIC ./Command)
add_subdirectory(Command)
