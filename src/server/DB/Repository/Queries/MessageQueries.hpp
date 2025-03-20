#ifndef MESSAGEQUERIES_HPP
#define MESSAGEQUERIES_HPP

#include <string>
#include <vector>

enum MessageQueriesEnum {INSERT_MESSAGE, GET_MESSAGES, GET_FROM, GET_TO, LIMIT_MESS_50};

const std::vector<std::string> MessageQueries = {
    "insert into messages(sender, recepient, content) values ()",
    "select * from messages",
    "select * from messages where sender = ",
    "select * from messages where recepient = ",
    "limit 50"
};

#endif