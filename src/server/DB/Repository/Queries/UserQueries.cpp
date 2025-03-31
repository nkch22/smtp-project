#ifndef USERQUERIES_CPP
#define USERQUERIES_CPP
#include <vector>
#include <string>

#include "../../Entity/User.hpp"


namespace UserQueryString {
    std::string insert_user(User user) {
        return "insert into users(id, name, password) values (" + std::to_string(user.id) + ", '" + user.name + "', '" + user.password + "');";
    }

    std::string select_users() {
        return "select * from users";
    }

    std::string select_by_id(int id) {
        return "select * from users where id = " + std::to_string(id) + ";";
    }

    std::string select_by_name(std::string name) {
        return "select * from users where name = '" + name + "';";
    }

    std::string limit_50() {
        return "limit 50";
    }

    std::string update_user(User user) {
        return "update users set (name, password)=('" + user.name + "', '" + user.password + "') where id=" + std::to_string(user.id) + ";";
    }

    std::string remove_user(User user) {
        return "delete from users where id=" + std::to_string(user.id) + ";";
    }
}

#endif