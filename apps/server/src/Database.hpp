#pragma once

#include "SMTP/Database/IDatabase.hpp"
#include "DB/Repository/Interface/UserRepo.hpp"
#include "DB/Repository/RepoPq/UserRepoPq.hpp"
#include "SMTP/Mailbox.hpp"

using SMTP::Database::IDatabase;
using SMTP::Database::IUser;
using SMTP::Mailbox;

class Database final : public IDatabase
{
public:
    using OptionalUser = IDatabase::OptionalUser;
    using OptionalMessage = IDatabase::OptionalMail;

    Database(std::string connection_str);
    ~Database() = default;

    OptionalUser ReadUser(const Mailbox& mailbox) const override;
    bool CreateUser(const IUser& user) override;
    bool UpdateUser(const IUser& user) override;
    bool RemoveUser(const IUser& user) override;
private:
    std::shared_ptr<UserRepo> users;
};