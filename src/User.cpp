#include "User.hpp"

User::User(const int id, const std::string& name, const std::string& password)
    : m_id{id}, m_name{name}
    , m_password{password}
{
}

int User::GetID() const
{
    return m_id;
}

std::string User::GetName() const
{
    return m_name;
}

std::string User::GetPassword() const
{
    return m_password;
}