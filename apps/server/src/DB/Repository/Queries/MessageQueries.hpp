#ifndef MESSAGEQUERIES_HPP
#define MESSAGEQUERIES_HPP

#include <string>
#include <vector>

#include "../../Entity/Message.hpp"

namespace MessageQueryString {
    std::string insert_message(Message message);

    std::string select_messages();

    std::string select_from(int id);

    std::string select_to(int id);

    std::string limit_50();

}

#endif