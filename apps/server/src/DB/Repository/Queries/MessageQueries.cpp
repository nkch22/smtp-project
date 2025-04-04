#ifndef MESSAGEQUERIES_CPP
#define MESSAGEQUERIES_CPP
#include <vector>
#include <string>

#include "../../Entity/Message.hpp"
#include "MessageQueries.hpp"

namespace MessageQueryString {
    std::string insert_message(Message message) {
        return "insert into messages(sender, recipient, content) values ("+std::to_string(message.from.id) + ", "+std::to_string(message.to.id)+", "+message.content+")";
    }

    std::string select_messages() {
        return "select * from messages";
    }

    std::string select_from(int id) {
        return "select * from messages where sender ="+std::to_string(id);
    }

    std::string select_to(int id) {
        return "select * from messages where recipient ="+std::to_string(id);
    }

    std::string limit_50() {
        return "limit 50";
    }
}


#endif