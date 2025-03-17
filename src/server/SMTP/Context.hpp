#pragma once
/**
 * @file Options.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <string>

#include "Transaction.hpp"

namespace SMTP
{

class Context
{
public:
    Transaction transaction;
    std::string domain_name;
    std::size_t max_message_size;
    bool plain_login_allowed;
    bool is_secure;
};

}