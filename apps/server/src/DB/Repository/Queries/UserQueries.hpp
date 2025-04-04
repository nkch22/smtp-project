#ifndef USERQUERIES_HPP
#define USERQUERIES_HPP
#include <vector>
#include <string>

#include "../../Entity/User.hpp"

namespace UserQueryString {
    std::string insert_user(User user);

    std::string select_users();

    std::string select_by_id(int id);

    std::string select_by_name(std::string name);

    std::string limit_50();

    std::string update_user(User user);

    std::string remove_user(User user);
}

#endif