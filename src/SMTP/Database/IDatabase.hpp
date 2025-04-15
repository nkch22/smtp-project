#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "IUser.hpp"
#include "IMail.hpp"

#include "../Mailbox.hpp"

namespace SMTP
{

namespace Database
{

class IDatabase
{
public:
    using OptionalUser = std::optional<std::unique_ptr<IUser>>;
    using OptionalMail = std::optional<std::unique_ptr<IMail>>;

    virtual ~IDatabase() = default;

    virtual OptionalUser ReadUser(const Mailbox& mailbox) = 0;
    virtual OptionalUser ReadUser(const int user_id) = 0;

    virtual std::vector<OptionalUser> ReadAllUsers() = 0;

    virtual bool CreateUser(const Mailbox& mailbox, const std::string& password) = 0;
    virtual bool UpdateUser(const int id, const std::string& name, const std::string& password) = 0;
    virtual bool UpdateUser(const IUser& user) = 0;
    virtual bool RemoveUser(const IUser& user) = 0;

    virtual std::vector<OptionalMail> ReadAllMails() = 0;

    virtual std::vector<OptionalMail> ReadMailsFrom(const IUser& user) = 0;
    virtual std::vector<OptionalMail> ReadMailsTo(const IUser& user) = 0;

    virtual bool CreateMail(const IUser& sender, const IUser& recepient, const std::string& message) = 0;
    virtual bool RemoveMail(const IMail& user) = 0;
};

}

}