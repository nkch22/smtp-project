#pragma once

#include "SMTP/Database/IUser.hpp"

class User final : public SMTP::Database::IUser
{
public:
    User(const int id, const std::string& name, const std::string& password);
    ~User() = default;

    int GetID() const override;
    std::string GetName() const override;
    std::string GetPassword() const override;
private:
    int m_id;
    std::string m_name;
    std::string m_password;
};