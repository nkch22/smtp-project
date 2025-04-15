#include "Database.hpp"

#include <format>

#include "User.hpp"
#include "Mail.hpp"
#include "PGQueryFormat.hpp"

#include "Logger/Logger.hpp"

Database::Database(const std::string& connection_string)
    : m_connection{connection_string}
{
}

Database::OptionalUser Database::ReadUser(const SMTP::Mailbox& mailbox)
{
    pqxx::work tx{m_connection};
    try
    {
        const auto query{std::format(
            PGQueryFormat::SELECT_USER_BY_NAME,
            tx.esc(mailbox.GetFullAddress())
        )};
        const auto result{tx.exec(query)};
        if(!result.empty())
        {
            return CreateUser(result[0]);
        }
    }
    catch(...)
    {
        Logger::error("Failed to read user by name from database: {}", mailbox.GetFullAddress());
        return std::nullopt;
    }
    
    return std::nullopt;
}

Database::OptionalUser Database::ReadUser(const int user_id)
{
    pqxx::work tx{m_connection};
    try
    {
        const auto query{std::format(
            PGQueryFormat::SELECT_USER_BY_ID,
            tx.esc(std::to_string(user_id))
        )};
        const auto result{tx.exec(query)};
        if(!result.empty())
        {
            return CreateUser(result[0]);
        }
    }
    catch(...)
    {
        Logger::error("Failed to read user by id from database: {}", user_id);
        return std::nullopt;
    }
    
    return std::nullopt;   
}

std::vector<Database::OptionalUser> Database::ReadAllUsers()
{
    pqxx::work tx{m_connection};
    try
    {
        const auto query{std::format("{} {}", 
            PGQueryFormat::SELECT_ALL_USERS, 
            PGQueryFormat::LIMIT
        )};
        std::vector<Database::OptionalUser> users{};
        for(const auto& user : tx.exec(query))
        {
            users.push_back(CreateUser(user));
        }
        return users;
    }
    catch(...)
    {
        Logger::error("Failed to read all users from database.");
        return {};
    }

    return {};
}

bool Database::CreateUser(const SMTP::Mailbox& user, const std::string& password)
{
    pqxx::work tx{m_connection};
    try 
    {
        const auto query{std::format(
            PGQueryFormat::INSERT_USER,
            tx.esc(user.GetFullAddress()),
            tx.esc(password)
        )};
        tx.exec(query);
    }
    catch(...)
    {
        Logger::error("Failed to create user in database. {}", user.GetFullAddress());
        return false;
    }
    tx.commit();
    return true;
}

bool Database::UpdateUser(const IUser& user)
{ 
    pqxx::work tx{m_connection};
    try 
    {
        const auto query{std::format(
            PGQueryFormat::UPDATE_USER,
            tx.esc(user.GetName()),
            tx.esc(user.GetPassword()),
            tx.esc(std::to_string(user.GetID()))
        )};
        const auto result{tx.exec(query)};
        tx.commit();
        return result.affected_rows() > 0;
    }
    catch(...) 
    {
        Logger::error("Failed to update user in database. User({};{})", user.GetID(), user.GetName());
        return false;
    }
    return false;
}


bool Database::UpdateUser(const int id, const std::string& name, const std::string& password)
{
    const User user{id, name, password};
    return UpdateUser(user);
}

bool Database::RemoveUser(const IUser& user)
{
    pqxx::work tx{m_connection};
    try 
    {
        const auto query{std::format(
            PGQueryFormat::REMOVE_USER,
            tx.esc(std::to_string(user.GetID()))
        )};
        const auto result{tx.exec(query)};
        tx.commit();
        return result.affected_rows() > 0; 
    }
    catch(...) 
    {
        Logger::error("Failed to remove user from database. User({};{})", user.GetID(), user.GetName());
        return false;
    }

    return false;
}

std::vector<Database::OptionalMail> Database::ReadAllMails()
{
    pqxx::work tx{m_connection};
    try
    {
        std::vector<Database::OptionalMail> mails{};
        const auto query{std::format(
            "{} {}",
            PGQueryFormat::SELECT_ALL_MAILS,
            PGQueryFormat::LIMIT
        )};
        for(const auto& mail : tx.exec(query))
        {
            mails.push_back(CreateMail(mail));
        }
    }
    catch(...)
    {
        Logger::error("Failed to read all mails from database.");
        return {};
    }
    return {};
}

std::vector<Database::OptionalMail> Database::ReadMailsFrom(const IUser& user)
{
    pqxx::work tx{m_connection};
    try 
    {
        std::vector<Database::OptionalMail> mails{};
        const auto mail_query{std::format( 
            PGQueryFormat::SELECT_MAIL_FROM, 
            tx.esc(std::to_string(user.GetID()))
        )};
        const auto query{std::format(
            "{} {}",
            mail_query,
            PGQueryFormat::LIMIT
        )};
        for(const auto& mail : tx.exec(query))
        {
            mails.push_back(CreateMail(mail));
        }
        return mails;
    }
    catch(...)
    {
        Logger::error("Failed to read mails from user from database. User({};{})", user.GetID(), user.GetName());
        return {};
    }
    return {};
}    

std::vector<Database::OptionalMail> Database::ReadMailsTo(const IUser& user)
{
    pqxx::work tx{m_connection};
    try 
    {
        std::vector<Database::OptionalMail> mails{};
        const auto mail_query{std::format( 
            PGQueryFormat::SELECT_MAIL_TO, 
            tx.esc(std::to_string(user.GetID()))
        )};
        const auto query{std::format(
            "{} {}",
            mail_query,
            PGQueryFormat::LIMIT
        )};
        for(const auto& mail : tx.exec(query))
        {
            mails.push_back(CreateMail(mail));
        }
        return mails;
    }
    catch(...)
    {
        Logger::error("Failed to read mails to user from database. User({};{})", user.GetID(), user.GetName());
        return {};
    }
    return {};
}

bool Database::CreateMail(const IUser& sender, const IUser& recepient, const std::string& message)
{
    pqxx::work tx{m_connection};
    try 
    {
        const auto query{std::format(
            PGQueryFormat::INSERT_MAIL,
            tx.esc(std::to_string(sender.GetID())),
            tx.esc(std::to_string(recepient.GetID())),
            tx.esc(message)
        )};
        tx.exec(query);
        tx.commit();
        return true;
    }
    catch(...)
    {
        Logger::error("Failed to create mail from {} {}", sender.GetID(), sender.GetName());
        return false;
    }
    return false;
}

bool Database::RemoveMail(const IMail& mail)
{
    pqxx::work tx{m_connection};
    try
    {
        const auto query{std::format(
            PGQueryFormat::REMOVE_MAIL,
            tx.esc(std::to_string(mail.GetID()))
        )};
        const auto result{tx.exec(query)};
        tx.commit();
        return result.affected_rows() > 0;
    }
    catch(...)
    {
        Logger::error(
            "Failed to remove mail from. Mail({};{};{};{})", 
            mail.GetID(), mail.GetMessage(), 
            mail.GetUserFromID(), mail.GetUserToID()
        );
        return false;
    }
    return true;
}

std::unique_ptr<IUser> Database::CreateUser(const pqxx::row& row)
{
    return std::make_unique<User>(
        row["id"].as<int>(),
        row["name"].as<std::string>(),
        row["password"].as<std::string>()
    );
}

std::unique_ptr<IMail> Database::CreateMail(const pqxx::row& row)
{
    return std::make_unique<Mail>(
        row["id"].as<int>(),
        row["sender_id"].as<int>(),
        row["recepient_id"].as<int>(),
        row["content"].as<std::string>()
    );
}