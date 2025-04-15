#pragma once

#include <string_view>

namespace PGQueryFormat
{
    using namespace std::string_view_literals;

    constexpr inline std::string_view INSERT_USER{"insert into users(name, password) values ('{}', '{}');"sv};
    constexpr inline std::string_view UPDATE_USER{"update users set (name, password)=('{}', '{}') where id={};"sv};
    constexpr inline std::string_view REMOVE_USER{"delete from users where id = {};"sv};

    constexpr inline std::string_view SELECT_ALL_USERS{"select * from users"sv};
    constexpr inline std::string_view SELECT_USER_BY_ID{"select * from users where id = {};"sv};
    constexpr inline std::string_view SELECT_USER_BY_NAME{"select * from users where name = '{}';"sv};

    constexpr inline std::string_view INSERT_MAIL{"insert into messages(sender_id, recipient_id, content) values ({}, {}, '{}');"sv};
    constexpr inline std::string_view REMOVE_MAIL{"delete from messages where id={};"sv};

    constexpr inline std::string_view SELECT_ALL_MAILS{"select * from messages;"sv};
    constexpr inline std::string_view SELECT_MAIL_FROM{"select * from messages where sender_id = {};"sv};
    constexpr inline std::string_view SELECT_MAIL_TO{"select * from messages where recepient_id = {};"sv};

    constexpr inline std::string_view LIMIT{"limit 50;"sv};
}