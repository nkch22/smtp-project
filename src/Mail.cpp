#include "Mail.hpp"

Mail::Mail(const int id, const int user_from_id, const int user_to_id, const std::string& message)
    : m_id{id}, m_user_from_id{user_from_id}
    , m_user_to_id{user_to_id}, m_message{message}
{
}

int Mail::GetID() const
{
    return m_id;
}

int Mail::GetUserFromID() const
{
    return m_user_from_id;
}

int Mail::GetUserToID() const
{
    return m_user_to_id;
}

std::string Mail::GetMessage() const
{
    return m_message;
}
