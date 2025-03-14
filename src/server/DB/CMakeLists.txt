add_library(db STATIC DBConnector.cpp Entity/UserRepository.cpp Entity/MessageRepository.cpp)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(db PRIVATE ${PQXX_LIBRARIES})