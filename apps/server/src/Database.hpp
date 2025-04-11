#pragma once

#include "SMTP/Database/IDatabase.hpp"
#include "DB/Repository/Interface/UserRepo.hpp"
#include "DB/Repository/RepoPq/UserRepoPq.hpp"
#include "DB/Repository/Interface/MessageRepo.hpp"
#include "DB/Repository/RepoPq/MessageRepoPq.hpp"
#include "SMTP/Mailbox.hpp"

using SMTP::Database::IDatabase;
using SMTP::Database::IUser;
using SMTP::Database::IMail;
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

    bool CreateMail(const IMail& message) override;
    std::vector<std::optional<IMail*>> GetReceivedMails(const IUser& user) override;
    std::vector<std::optional<IMail*>> GetSentMails(const IUser& user) override;
private:
    std::shared_ptr<UserRepo> users;
    std::shared_ptr<MessageRepo> mails;

};