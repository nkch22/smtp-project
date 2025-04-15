#pragma once

#include <string>
#include <memory>
#include <optional>

#include "Transaction.hpp"
#include "Database/IDatabase.hpp"
#include "Protocol/Command/AuthentificationMethod.hpp"

namespace SMTP
{

struct Context
{
    Context()
        : waiting_auth{std::nullopt}, 
        is_auth_login_username_received{false}
    {
    }

    std::shared_ptr<Database::IDatabase> database;
    Transaction transaction;
    std::string domain_name;
    std::size_t max_message_size;
    bool plain_login_allowed;
    bool is_secure;

    std::optional<Protocol::AuthentificationMethod> waiting_auth;
    bool is_auth_login_username_received;
};

}