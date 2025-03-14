#include "UserRepository.hpp"

#include <optional>

UserRepository::UserRepository(DBConnector* dbc) :
	m_dbc(dbc) {}

UserRepository::~UserRepository()
{
	m_dbc = nullptr;
}

bool UserRepository::CreateUser(const User& user) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	try
	{
		tx.exec("insert into users(id, name, password) values (" + std::to_string(user.id) + ", '" + user.name + "', '"
		        + user.password + "')");
	}

	catch (...)
	{
		return false;
	}
	tx.commit();
	return true;
}

std::vector<std::optional<User>> UserRepository::GetUsers() const
{
	pqxx::work tx{m_dbc->GetConnection()};
	std::vector<std::optional<User>> users;
	for (auto user : tx.exec("select * from users"))
	{
		if (user.size() > 0)
			users.emplace_back(User{.id = user["id"].as<int>(),
			                        .name = user["name"].as<std::string>(),
			                        .password = user["password"].as<std::string>()});
	}
	tx.commit();
	return users;
}

std::optional<User> UserRepository::GetById(int id) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	auto user = tx.exec("select * from users where id = " + std::to_string(id));
	tx.commit();
	if (user.size() > 0)
		return User{.id = user[0]["id"].as<int>(),
		            .name = user[0]["name"].as<std::string>(),
		            .password = user[0]["password"].as<std::string>()};
	return std::nullopt;
}

std::optional<User> UserRepository::GetByName(const std::string& name) const
{
	pqxx::work tx{m_dbc->GetConnection()};
	auto user = tx.exec("select * from users where name = '" + name + "'");
	tx.commit();
	if (user.size() > 0)
		return User{.id = user[0]["id"].as<int>(),
		            .name = user[0]["name"].as<std::string>(),
		            .password = user[0]["password"].as<std::string>()};
	return std::nullopt;
}