#pragma once

#include "SMTP/Database/IUser.hpp"

using SMTP::Database::IUser;

class User : public IUser
{
public:
    ~User() = default;
};