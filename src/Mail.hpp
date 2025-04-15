#pragma once

#include "SMTP/Database/IMail.hpp"

class Mail final : public SMTP::Database::IMail
{
public:
    Mail(const int id, const int user_from_id, const int user_to_id, const std::string& message);
    ~Mail() = default;

    int GetID() const override;
    int GetUserFromID() const override;
    int GetUserToID() const override;
    std::string GetMessage() const override;
private:
    int m_id;
    int m_user_from_id;
    int m_user_to_id;
    std::string m_message;
};