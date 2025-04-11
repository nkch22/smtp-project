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

  bool CreateMessage(const Message& msg) override;

  std::vector<std::optional<Message>> GetMessages() override;
  std::vector<std::optional<Message>> GetMessagesFrom(const User& user) override;
  std::vector<std::optional<Message>> GetMessagesTo(const User& user) override;
};

#endif
