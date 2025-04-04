#ifndef USERREPO_HPP
#define USERREPO_HPP

#include "../../Entity/User.hpp"
#include <vector>
#include <optional>


class UserRepo {
    public:
        virtual bool CreateUser(const User& user) = 0;
        virtual bool UpdateUser(const User& user) = 0;
        virtual bool RemoveUser(const User& user) = 0;
        
        virtual std::vector<std::optional<User>> GetUsers() = 0;
        virtual std::optional<User> GetById(int id) = 0;
        virtual std::optional<User> GetByName(std::string name) = 0;
};


#endif
