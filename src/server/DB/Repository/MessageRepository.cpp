#include "MessageRepository.hpp"

#include <iostream>
#include <optional>

MessageRepository::MessageRepository(DBConnector* dbc) : m_dbc(dbc) {}

MessageRepository::~MessageRepository()
{
	m_dbc = nullptr;
}

bool MessageRepository::CreateMessage(const Message& msg) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	try
	{
		tx.exec("insert into messages(sender, recepient, content) values (" + std::to_string(msg.from.id) + ", "
				+ std::to_string(msg.to.id) + ", '" + msg.content + "')");
	}
	catch (...)
	{
		return false;
	}
	tx.commit();
	return true;
}

std::vector<std::optional<Message>> MessageRepository::GetMessages() const
{
	pqxx::work tx{m_dbc->GetConnection()};
	std::vector<std::optional<Message>> messages;
	for (auto msg : tx.exec("select * from messages"))
	{
		auto usrfrom = tx.exec("select * from users where id = " + msg["sender"].as<std::string>()).one_row();
		auto usrto = tx.exec("select * from users where id = " + msg["recepient"].as<std::string>()).one_row();
		messages.emplace_back(Message{.from = User{.id = usrfrom["id"].as<int>(),
												   .name = usrfrom["name"].as<std::string>(),
												   .password = usrfrom["password"].as<std::string>()},
									  .to = User{.id = usrto["id"].as<int>(),
												 .name = usrto["name"].as<std::string>(),
												 .password = usrto["password"].as<std::string>()},
									  .content = msg["content"].as<std::string>()});
	}
	tx.commit();
	return messages;
}

std::vector<std::optional<Message>> MessageRepository::GetMessagesFrom(const User& user) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	auto check = tx.exec("select * from users where id = " + std::to_string(user.id));
	auto messages = std::vector<std::optional<Message>>();
	if (!check.empty())
	{
		std::cout << "inside if\n";
		for (auto msg : tx.exec("select * from messages where sender = " + std::to_string(user.id)))
		{
			auto usrto = tx.exec("select * from users where id = " + msg["recepient"].as<std::string>()).one_row();
			messages.emplace_back(Message{.from = user,
										  .to = User{.id = usrto["id"].as<int>(),
													 .name = usrto["name"].as<std::string>(),
													 .password = usrto["password"].as<std::string>()},
										  .content = msg["content"].as<std::string>()});
		}
	}
	else
		std::cout << "User not found" << std::endl;
	tx.commit();
	return messages;
}

std::vector<std::optional<Message>> MessageRepository::GetMessagesTo(const User& user) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	auto check = tx.exec("select * from users where id = " + std::to_string(user.id));
	auto messages = std::vector<std::optional<Message>>();

	if (!check.empty())
	{
		for (auto msg : tx.exec("select * from messages where recepient = " + std::to_string(user.id)))
		{
			auto usrfrom = tx.exec("select * from users where id = " + msg["sender"].as<std::string>()).one_row();
			messages.emplace_back(Message{.from = User{.id = usrfrom["id"].as<int>(),
													   .name = usrfrom["name"].as<std::string>(),
													   .password = usrfrom["password"].as<std::string>()},
										  .to = user,
										  .content = msg["content"].as<std::string>()});
		}
	}
	else
		std::cout << "User not found" << std::endl;

	tx.commit();
	return messages;
}