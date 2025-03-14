add_library(db STATIC
        Connection/DBConnector.cpp
        Repository/UserRepository.cpp
        Repository/MessageRepository.cpp

        # make IDE happy
        Entity/Message.hpp
        Entity/User.hpp
        Repository/UserRepository.hpp
        Repository/MessageRepository.hpp
        Connection/DBConnector.hpp
)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(db PRIVATE pqxx)

# For test purposes
add_executable(db_exec main.cpp)
target_link_libraries(db_exec db)