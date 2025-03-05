#pragma once

#include "UserRepository.hpp"
#include <optional>
#include <iostream>

UserRepository::UserRepository(DBConnector* dbc): dbc(dbc) {}
UserRepository::~UserRepository() {dbc = nullptr;}

bool UserRepository::create_user(User user) {
    pqxx::work tx{dbc->conn};
    try {
        tx.exec("insert into users(id, name, password) values ("+std::to_string(user.id)+", '"+user.name+"', '"+user.password+"')");
    }
    
    catch(...) {return false;}
    tx.commit();
    return true;
}

std::vector<std::optional<User>> UserRepository::get_users() {
    pqxx::work tx{dbc->conn};
    std::vector<std::optional<User>> users;
    for (auto user: tx.exec("select * from users")) {
        if (!user.empty())
            users.push_back(User{user["id"].as<int>(), user["name"].as<std::string>(), user["password"].as<std::string>()});
    }
    tx.commit();
    return users;
}

std::optional<User> UserRepository::get_by_id(int id) {
    pqxx::work tx{dbc->conn};
    auto user = tx.exec("select * from users where id = "+std::to_string(id));
    tx.commit();
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}

std::optional<User> UserRepository::get_by_name(std::string name) {
    pqxx::work tx{dbc->conn};
    auto user = tx.exec("select * from users where name = '"+name+"'");
    tx.commit();
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}


