#ifndef MESSAGEREPOPQ_CPP
#define MESSAGEREPOPQ_CPP


#include "../Queries/UserQueries.hpp"
#include "../Queries/MessageQueries.hpp"
#include "MessageRepoPq.hpp"
#include <optional>
#include <string>
#include <iostream>

MessageRepoPq::MessageRepoPq(std::string connection_str): conn(connection_str) {}

bool MessageRepoPq::CreateMessage(const Message& msg) {
    pqxx::work tx{conn};
    std::string query_str = MessageQueryString::insert_message(msg);
    try {
        tx.exec(query_str);
        tx.commit();
        return true;
    }
    catch(...) {return false;}
    
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessages() {
    pqxx::work tx{conn};
    std::string query_str = MessageQueryString::select_messages()+" "+MessageQueryString::limit_50();
    std::vector<std::optional<Message>> messages;
    for (auto msg: tx.exec(query_str)) {
        auto usrfrom = tx.exec1(UserQueryString::select_by_id(msg["sender"].as<int>()));
        auto usrto = tx.exec1(UserQueryString::select_by_id(msg["recipient"].as<int>()));
        messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
            User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
            msg["content"].as<std::string>()});
    }
    return messages;
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessagesFrom(const User& user) {
    //this function only checks if id is valid
    pqxx::work tx{conn};
    auto check = tx.exec(UserQueryString::select_by_id(user.id));
    auto messages = std::vector<std::optional<Message>>();
    if (!check.empty()) {
        for (auto msg: tx.exec(MessageQueryString::select_from(user.id)+" "
                                            +MessageQueryString::limit_50())) {
            auto usrto = tx.exec1(UserQueryString::select_by_id(msg["recipient"].as<int>()));
            messages.push_back(Message{user,
                User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;
    return messages;
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessagesTo(const User& user) {
    //this function only checks if id is valid
    pqxx::work tx{conn};
    auto check = tx.exec(UserQueryString::select_by_id(user.id));
    auto messages = std::vector<std::optional<Message>>();

    if (!check.empty()) {
        for (auto msg: tx.exec(MessageQueryString::select_to(user.id)+" "
                        +MessageQueryString::limit_50())) {
            auto usrfrom = tx.exec1(UserQueryString::select_by_id(msg["sender"].as<int>()));
            messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
                user,
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;

    return messages;
}

#endif
