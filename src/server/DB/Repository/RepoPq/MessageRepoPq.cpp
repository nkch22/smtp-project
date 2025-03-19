#ifndef MESSAGEREPOPQ_CPP
#define MESSAGEREPOPQ_CPP


#include "../Queries/UserQueries.hpp"
#include "../Queries/MessageQueries.hpp"
#include "MessageRepoPq.hpp"
#include <optional>
#include <string>
#include <iostream>

MessageRepoPq::MessageRepoPq(std::string connection_str): conn(connection_str) {}

bool MessageRepoPq::CreateMessage(Message msg) {
    pqxx::work tx{conn};
    std::string query_str = MessageQueries[MessageQueriesEnum::INSERT_MESSAGE];
    try {
        tx.exec(query_str.substr(0, query_str.length()-1)+std::to_string(msg.from.id)+", "+std::to_string(msg.to.id)+", '"+msg.content+"'"+query_str[query_str.length()-1]);
    }
    catch(...) {return false;}
    tx.commit();
    return true;
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessages() {
    pqxx::work tx{conn};
    std::string query_str = MessageQueries[MessageQueriesEnum::GET_MESSAGES]+" "+MessageQueries[MessageQueriesEnum::LIMIT_MESS_50];
    std::vector<std::optional<Message>> messages;
    for (auto msg: tx.exec(query_str)) {
        auto usrfrom = tx.exec1(UserQueries[UserQueriesEnum::GET_BY_ID]+msg["sender"].as<std::string>());
        auto usrto = tx.exec1(UserQueries[UserQueriesEnum::GET_BY_ID]+msg["recepient"].as<std::string>());
        messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
            User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
            msg["content"].as<std::string>()});
    }
    return messages;
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessagesFrom(User user) {
    pqxx::work tx{conn};
    auto check = tx.exec(UserQueries[UserQueriesEnum::GET_BY_ID]+std::to_string(user.id));
    auto messages = std::vector<std::optional<Message>>();
    if (!check.empty()) {
        for (auto msg: tx.exec(MessageQueries[MessageQueriesEnum::GET_FROM]+std::to_string(user.id)+" "
                                            +MessageQueries[MessageQueriesEnum::LIMIT_MESS_50])) {
            auto usrto = tx.exec1(UserQueries[UserQueriesEnum::GET_BY_ID]+msg["recepient"].as<std::string>());
            messages.push_back(Message{user,
                User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;
    return messages;
}

std::vector<std::optional<Message>> MessageRepoPq::GetMessagesTo(User user) {
    pqxx::work tx{conn};
    auto check = tx.exec(UserQueries[UserQueriesEnum::GET_BY_ID]+std::to_string(user.id));
    auto messages = std::vector<std::optional<Message>>();

    if (!check.empty()) {
        for (auto msg: tx.exec(MessageQueries[MessageQueriesEnum::GET_TO]+std::to_string(user.id)+" "
                        +MessageQueries[MessageQueriesEnum::LIMIT_MESS_50])) {
            auto usrfrom = tx.exec1(UserQueries[UserQueriesEnum::GET_BY_ID]+msg["sender"].as<std::string>());
            messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
                user,
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;

    return messages;
}

#endif
