#include "Database.hpp"
#include <optional>

#include <iostream>

Database::Database(std::string connection_str): users(new UserRepoPq(connection_str)) {}

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