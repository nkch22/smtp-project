#pragma once

#include "IDatabase.hpp"
#include "../../DB/Repository/Interface/UserRepo.hpp"
#include "../../DB/Repository/RepoPq/UserRepoPq.hpp"

// #include "../../DB/Repository/Interface/MessageRepo.hpp"


namespace SMTP {
    namespace Database {
        class Database: public IDatabase {
            public:
                // Database();
                Database(std::string connection_str);
                ~Database() = default;

                OptionalUser ReadUser(const Mailbox& mailbox) const override;
                bool CreateUser(const IUser& user) override;
                bool UpdateUser(const IUser& user) override;
                bool RemoveUser(const IUser& user) override;

            private:
                std::shared_ptr<UserRepo> users;
                // std::shared_ptr<MessageRepo> messages;
        };
    }
}