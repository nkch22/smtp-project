#ifndef MESSAGEREPO_HPP
#define MESSAGEREPO_HPP

#include "../../Entity/Message.hpp"
#include <optional>
#include <vector>

class MessageRepo {
    public:
        virtual bool CreateMessage(Message msg) = 0;
        virtual std::vector<std::optional<Message>> GetMessages() = 0;
        virtual std::vector<std::optional<Message>> GetMessagesFrom(User user) = 0;
        virtual std::vector<std::optional<Message>> GetMessagesTo(User user) = 0;
};


#endif
