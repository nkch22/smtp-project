#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "User.hpp"
#include "../../SMTP/Database/IMail.hpp"
#include <string>

struct Message: public SMTP::Database::IMail {
    User from;
    User to;
    std::string content;
    Message(User from, User to, std::string content): from(from), to(to), content(content) {}
  };

#endif