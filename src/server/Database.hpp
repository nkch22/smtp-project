#pragma once

#include "SMTP/Database/IDatabase.hpp"

class Database final : public SMTP::Database::IDatabase
{
public:
    void ReadUser() override;
    void ReadEmail() override;

    void CreateUser() override;
    void CreateEmail() override;

    void UpdateUser() override;
    void UpdateEmail() override;

    void RemoveUser() override;
    void RemoveEmail() override;
private:
};