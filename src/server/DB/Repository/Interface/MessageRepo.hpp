#ifndef MESSAGEREPO_HPP
#define MESSAGEREPO_HPP

#include "../../Entity/Message.hpp"
#include <optional>
#include <vector>

class MessageRepo {
    public:
        virtual bool create_message(Message msg) = 0;
        virtual std::vector<std::optional<Message>> get_messages() = 0;
        virtual std::vector<std::optional<Message>> get_messages_from(User user) = 0;
        virtual std::vector<std::optional<Message>> get_messages_to(User user) = 0;
};


#endif
