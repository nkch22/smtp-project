add_executable(main main.cpp Database.cpp ../Mailbox.cpp)

find_library(DB_FOUND NAMES db PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../DB/build)

if(NOT DB_FOUND) 
    message(FATAL_ERROR "Couldn't find db library")
endif()

# message(STATUS "Found DB library: ${DB_FOUND}")

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../DB/build)

target_link_libraries(main PRIVATE ${DB_FOUND} pqxx)