#ifndef USERREPO_HPP
#define USERREPO_HPP

#include "../../Entity/User.hpp"
#include <vector>
#include <optional>


class UserRepo {
    public:
        virtual bool CreateUser(User user) = 0;
        virtual std::vector<std::optional<User>> GetUsers() = 0;
        virtual std::optional<User> GetById(int id) = 0;
        virtual std::optional<User> GetByName(std::string name) = 0;
};


#endif
