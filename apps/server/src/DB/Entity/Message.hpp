#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "User.hpp"
#include <string>

struct Message {
    User from;
    User to;
    std::string content;
  };

#endif