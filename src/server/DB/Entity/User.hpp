#ifndef USER_HPP
#define USER_HPP
#include <string>

#include "../../SMTP/Database/IUser.hpp"

struct User: public SMTP::Database::IUser {
    int id;
    std::string name;
    std::string password;
    User(int id, std::string name, std::string password): id(id), name(name), password(password) {} 
  };
#endif