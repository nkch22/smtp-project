#ifndef MESSAGEREPOPQ_HPP
#define MESSAGEREPOPQ_HPP


#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "../../Entity/Message.hpp"
#include "../Interface/MessageRepo.hpp"

class MessageRepoPq: public MessageRepo {
private:
  pqxx::connection conn;

public:
  MessageRepoPq(std::string connection_str);
  ~MessageRepoPq() = default;

  bool create_message(Message msg);

  std::vector<std::optional<Message>> get_messages();
  std::vector<std::optional<Message>> get_messages_from(User user);
  std::vector<std::optional<Message>> get_messages_to(User user);
};

#endif
