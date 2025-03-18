#pragma once

#include "SMTP/Database/IDatabase.hpp"
#include "SMTP/Mailbox.hpp"

using SMTP::Database::IDatabase;
using SMTP::Database::IUser;
using SMTP::Mailbox;

class Database final : public IDatabase
{
public:
    using OptionalUser = IDatabase::OptionalUser;
    using OptionalMessage = IDatabase::OptionalMail;

    ~Database() = default;

    OptionalUser ReadUser(const Mailbox& mailbox) const override;
    bool CreateUser(const IUser& user) override;
    bool UpdateUser(const IUser& user) override;
    bool RemoveUser(const IUser& user) override;
};