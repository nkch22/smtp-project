#pragma once

#include "UserRepository.hpp"
#include "../DBConnector.hpp"

struct Message {
  User from;
  User to;
  std::string content;
};

class MessageRepository {
private:
  DBConnector* dbc;
public:
  MessageRepository(DBConnector* dbc);
  ~MessageRepository();

  bool create_message(Message msg);

  std::vector<std::optional<Message>> get_messages();
  std::vector<std::optional<Message>> get_messages_from(User user);
  std::vector<std::optional<Message>> get_messages_to(User user);
};