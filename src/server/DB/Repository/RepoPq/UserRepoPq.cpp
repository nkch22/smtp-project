#ifndef USERREPOPQ_CPP
#define USERREPOPQ_CPP

#include "UserRepoPq.hpp"
#include "../Queries/UserQueries.hpp"
#include <optional>
#include <iostream>

UserRepoPq::UserRepoPq(std::string connection_str): conn(connection_str) {}

bool UserRepoPq::CreateUser(User user) {
    pqxx::work tx{conn};
    try {
        std::string query_str = UserQueries[UserQueriesEnum::INSERT_USER];
        tx.exec(query_str.substr(0, query_str.length()-1) + std::to_string(user.id)+", '"+user.name+"', '"+user.password+"'"+query_str[query_str.length()-1]);
    }
    
    catch(...) {return false;}
    tx.commit();
    return true;
}

std::vector<std::optional<User>> UserRepoPq::GetUsers() {
    pqxx::work tx{conn};
    std::string query_str = UserQueries[UserQueriesEnum::GET_USERS]+" "+UserQueries[UserQueriesEnum::LIMIT_USER_50];
    std::vector<std::optional<User>> users;
    for (auto user: tx.exec(query_str)) {
        if (!user.empty())
            users.push_back(User{user["id"].as<int>(), user["name"].as<std::string>(), user["password"].as<std::string>()});
    }
    return users;
}

std::optional<User> UserRepoPq::GetById(int id) {
    pqxx::work tx{conn};
    std::string query_str = UserQueries[UserQueriesEnum::GET_BY_ID];
    auto user = tx.exec(query_str+std::to_string(id));
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}

std::optional<User> UserRepoPq::GetByName(std::string name) {
    pqxx::work tx{conn};
    std::string query_str = UserQueries[UserQueriesEnum::GET_BY_NAME];
    auto user = tx.exec(query_str+"'"+name+"'");
    if (!user.empty())
        return User{user[0]["id"].as<int>(), user[0]["name"].as<std::string>(), user[0]["password"].as<std::string>()};
    return std::nullopt;
}

#endif
