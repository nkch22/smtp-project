#include "Database.hpp"
#include <optional>

#include <iostream>

Database::Database(std::string connection_str): users(new UserRepoPq(connection_str)), mails(new MessageRepoPq(connection_str)) {}

Database::OptionalUser Database::ReadUser(const Mailbox& mailbox) const
{
    auto user = users->GetByName(*(mailbox.get_user()) + '@' + *(mailbox.get_host())); //std::optional<User>
    if (!user) return std::nullopt;
    
    return std::make_unique<User>(*user);}

bool Database::CreateUser(const IUser& user)
{
    return users->CreateUser(dynamic_cast<const User&>(user));
}

bool Database::UpdateUser(const IUser& user)
{
    return users->UpdateUser(dynamic_cast<const User&>(user));
}

bool Database::RemoveUser(const IUser& user)
{
    return users->RemoveUser(dynamic_cast<const User&>(user));
}

bool Database::CreateMail(const IMail& message) {
    return mails->CreateMessage(dynamic_cast<const Message&>(message));
}


std::vector<std::optional<IMail*>> Database::GetReceivedMails(const IUser& user) {
    auto msgsto = mails->GetMessagesTo(dynamic_cast<const User&>(user));
    std::vector<std::optional<IMail*>> result;
    for (auto opt_msg : msgsto) {
        if (opt_msg) {
            result.push_back(new Message (opt_msg.value()));
        }

        else result.push_back(std::nullopt);
    }
    return result;
}

std::vector<std::optional<IMail*>> Database::GetSentMails(const IUser& user) {
    auto msgsfrom = mails->GetMessagesFrom(dynamic_cast<const User&>(user));
    std::vector<std::optional<IMail*>> result;
    for (auto opt_msg : msgsfrom) {
        if (opt_msg) {
            result.push_back(new Message (opt_msg.value()));
        }

        else result.push_back(std::nullopt);
    }
    return result;
}