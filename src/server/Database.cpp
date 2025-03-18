#include "Database.hpp"

Database::OptionalUser Database::ReadUser(const Mailbox& mailbox) const
{
    return std::nullopt;
}

bool Database::CreateUser(const IUser& user)
{
    return true;
}

bool Database::UpdateUser(const IUser& user)
{
    return true;
}

bool Database::RemoveUser(const IUser& user)
{
    return true;
}