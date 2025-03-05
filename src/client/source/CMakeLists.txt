file(GLOB client_source CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(client PRIVATE ${client_source})