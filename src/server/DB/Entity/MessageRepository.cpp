#pragma once

#include "MessageRepository.hpp"
#include <optional>
#include <iostream>

MessageRepository::MessageRepository(DBConnector* dbc): dbc(dbc) {}
MessageRepository::~MessageRepository() {dbc = nullptr;}

bool MessageRepository::create_message(Message msg) {
    pqxx::work tx{dbc->conn};
    try {
        tx.exec("insert into messages(sender, recepient, content) values ("+std::to_string(msg.from.id)+", "+std::to_string(msg.to.id)+", '"+msg.content+"')");
    }
    catch(...) {return false;}
    tx.commit();
    return true;
}

std::vector<std::optional<Message>> MessageRepository::get_messages() {
    pqxx::work tx{dbc->conn};
    std::vector<std::optional<Message>> messages;
    for (auto msg: tx.exec("select * from messages")) {
        auto usrfrom = tx.exec1("select * from users where id = "+msg["sender"].as<std::string>());
        auto usrto = tx.exec1("select * from users where id = "+msg["recepient"].as<std::string>());
        messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
            User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
            msg["content"].as<std::string>()});
    }
    tx.commit();
    return messages;
}

std::vector<std::optional<Message>> MessageRepository::get_messages_from(User user) {
    pqxx::work tx{dbc->conn};
    auto check = tx.exec("select * from users where id = "+std::to_string(user.id));
    auto messages = std::vector<std::optional<Message>>();
    if (!check.empty()) {
        std::cout<<"inside if\n";
        for (auto msg: tx.exec("select * from messages where sender = "+std::to_string(user.id))) {
            auto usrto = tx.exec1("select * from users where id = "+msg["recepient"].as<std::string>());
            messages.push_back(Message{user,
                User{usrto["id"].as<int>(), usrto["name"].as<std::string>(), usrto["password"].as<std::string>()},
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;
    tx.commit();
    return messages;
}

std::vector<std::optional<Message>> MessageRepository::get_messages_to(User user) {
    pqxx::work tx{dbc->conn};
    auto check = tx.exec("select * from users where id = "+std::to_string(user.id));
    auto messages = std::vector<std::optional<Message>>();

    if (!check.empty()) {
        for (auto msg: tx.exec("select * from messages where recepient = "+std::to_string(user.id))) {
            auto usrfrom = tx.exec1("select * from users where id = "+msg["sender"].as<std::string>());
            messages.push_back(Message{User{usrfrom["id"].as<int>(), usrfrom["name"].as<std::string>(), usrfrom["password"].as<std::string>()},
                user,
                msg["content"].as<std::string>()});
        }
    }
    else std::cout<<"User not found"<<std::endl;

    tx.commit();
    return messages;
}