add_subdirectory(JSON)
add_subdirectory(Config)
add_subdirectory(DB)
add_subdirectory(Initializer)
add_subdirectory(Logger)

add_library(smtp STATIC)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC asio )
target_link_libraries(smtp PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(smtp_server main.cpp Database.cpp Mail.cpp Server.cpp Session.cpp User.cpp)

add_dependencies(smtp_server copy_certificates)
target_link_libraries(smtp_server PRIVATE smtp)

target_link_libraries(smtp_server
    PRIVATE
    json
    config
    db
    initializer
)