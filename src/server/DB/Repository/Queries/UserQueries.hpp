#ifndef USERQUERIES_HPP
#define USERQUERIES_HPP
#include <vector>
#include <string>

enum UserQueriesEnum {INSERT_USER, GET_USERS, GET_BY_ID, GET_BY_NAME, LIMIT_USER_50};
const std::vector<std::string> UserQueries = {
    "insert into users(id, name, password) values ()",
    "select * from users",
    "select * from users where id = ",
    "select * from users where name = ",
    "limit 50"
};

#endif