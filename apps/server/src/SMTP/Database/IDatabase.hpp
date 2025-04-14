#pragma once

#include <optional>
#include <memory>

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

    virtual OptionalUser ReadUser(const Mailbox& mailbox) const = 0;
    virtual bool CreateUser(const IUser& user) = 0;
    virtual bool UpdateUser(const IUser& user) = 0;
    virtual bool RemoveUser(const IUser& user) = 0;

    virtual bool CreateMail(const IMail& message) = 0;
    virtual std::vector<std::optional<IMail*>> GetReceivedMails(const IUser& user) = 0;
    virtual std::vector<std::optional<IMail*>> GetSentMails(const IUser& user) = 0;
};

}

}