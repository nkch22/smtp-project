#pragma once

#include "Connection/DBConnector.hpp"
#include "Entity/Message.hpp"

class MessageRepository
{
private:
	DBConnector* m_dbc;

public:
	MessageRepository(DBConnector* dbc);
	~MessageRepository();

	bool CreateMessage(const Message& msg) const;

	std::vector<std::optional<Message>> GetMessages() const;
	std::vector<std::optional<Message>> GetMessagesFrom(const User& user) const;
	std::vector<std::optional<Message>> GetMessagesTo(const User& user) const;
};
