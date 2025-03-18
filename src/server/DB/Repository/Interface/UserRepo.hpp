#ifndef USERREPO_HPP
#define USERREPO_HPP

#include "../../Entity/User.hpp"
#include <vector>
#include <optional>


class UserRepo {
    public:
        virtual bool create_user(User user) = 0;
        virtual std::vector<std::optional<User>> get_users() = 0;
        virtual std::optional<User> get_by_id(int id) = 0;
        virtual std::optional<User> get_by_name(std::string name) = 0;
};


#endif
