#ifndef USERREPOPQ_CPP
#define USERREPOPQ_CPP

#include "UserRepoPq.hpp"
#include "../Queries/UserQueries.hpp"
#include <optional>
#include <iostream>

UserRepoPq::UserRepoPq(std::string connection_str): conn(connection_str) {}

bool UserRepoPq::CreateUser(const User& user) {
    pqxx::work tx{conn};
    try {
        std::string query_str = UserQueryString::insert_user(user);
        tx.exec(query_str);
    }
    
    catch(...) {return false;}
    tx.commit();
    return true;
}

bool UserRepoPq::UpdateUser(const User& user) {
    pqxx::work tx{conn};
    try {
        std::string query_str = UserQueryString::update_user(user);
        auto result = tx.exec(query_str);
        tx.commit();
        return result.affected_rows()>0;
    }
    catch (...) {return false;}
}

bool UserRepoPq::RemoveUser(const User& user) {
    pqxx::work tx{conn};
    try {
        std::string query_str = UserQueryString::remove_user(user);
        auto result = tx.exec(query_str);
        tx.commit();
        return result.affected_rows()>0;
    }
    catch (...) {return false;}

}

std::vector<std::optional<User>> UserRepoPq::GetUsers() {
    pqxx::work tx{conn};
    std::string query_str = UserQueryString::select_users()+" "+UserQueryString::limit_50();
    std::vector<std::optional<User>> users;
    for (auto user: tx.exec(query_str)) {
        if (!user.empty())
            users.push_back(User{user["id"].as<int>(), user["name"].as<std::string>(), user["password"].as<std::string>()});
    }
    return users;
}

std::optional<User> UserRepoPq::GetById(int id) {
    pqxx::work tx{conn};
    std::string query_str = UserQueryString::select_by_id(id);
    auto user = tx.exec(query_str);
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}

std::optional<User> UserRepoPq::GetByName(std::string name) {
    pqxx::work tx{conn};
    std::string query_str = UserQueryString::select_by_name(name);
    auto user = tx.exec(query_str);
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}

#endif
