#pragma once

#include "UserRepository.hpp"

struct Message {
  User from;
  User to;
  std::string content;
};

class MessageRepository {
public:
  // TODO:: connection
  MessageRepository() {}

  bool create_message(Message msg) {}

  std::vector<std::optional<User>> get_messages();
  std::vector<std::optional<Message>> get_messages_from(User user) {}
  std::vector<std::optional<Message>> get_messages_to(User user) {}
};