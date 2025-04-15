#pragma once

#include <string>

namespace SMTP
{

namespace Protocol
{

struct Options
{
    std::string domain_name;
    std::size_t max_message_size;
    bool plain_login_allowed;
    bool is_secure;
};

}

}