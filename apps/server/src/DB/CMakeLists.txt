add_library(db STATIC
        Repository/RepoPq/UserRepoPq.cpp
        Repository/RepoPq/MessageRepoPq.cpp
        Repository/Queries/UserQueries.cpp
        Repository/Queries/MessageQueries.cpp

        # make IDE happy
        Entity/Message.hpp
        Entity/User.hpp
        Repository/Queries/UserQueries.hpp
        Repository/Queries/MessageQueries.hpp
        Repository/Interface/UserRepo.hpp
        Repository/Interface/MessageRepo.hpp
        Repository/RepoPq/UserRepoPq.hpp
        Repository/RepoPq/MessageRepoPq.hpp
)

target_include_directories(db PUBLIC .)
target_link_libraries(db PRIVATE pqxx)

# For test purposes
add_executable(db_exec main.cpp)
target_link_libraries(db_exec db)
