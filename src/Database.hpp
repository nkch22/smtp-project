#pragma once

#include <string>

#include <pqxx/pqxx>

#include "SMTP/Database/IDatabase.hpp"

using SMTP::Database::IDatabase;
using SMTP::Database::IMail;
using SMTP::Database::IUser;

class Database final : public IDatabase
{
public:
    using OptionalUser = IDatabase::OptionalUser;
    using OptionalMail = IDatabase::OptionalMail;

    Database(const std::string& connection_string);
    ~Database() = default;

    OptionalUser ReadUser(const SMTP::Mailbox& mailbox) override;
    OptionalUser ReadUser(const int user_id) override;

    std::vector<OptionalUser> ReadAllUsers() override;
    
    bool CreateUser(const SMTP::Mailbox& mailbox, const std::string& password) override;
    bool UpdateUser(const IUser& user) override;
    bool UpdateUser(const int id, const std::string& name, const std::string& password) override;
    bool RemoveUser(const IUser& user) override;
    
    std::vector<OptionalMail> ReadMailsFrom(const IUser& user) override;
    std::vector<OptionalMail> ReadMailsTo(const IUser& user) override;
    std::vector<OptionalMail> ReadAllMails() override;
    
    bool CreateMail(const IUser& sender, const IUser& recepient, const std::string& message) override;
    bool RemoveMail(const IMail& user) override;
private:
    static std::unique_ptr<IUser> CreateUser(const pqxx::row& user_row);
    static std::unique_ptr<IMail> CreateMail(const pqxx::row& mail_row);

    pqxx::connection m_connection;
};