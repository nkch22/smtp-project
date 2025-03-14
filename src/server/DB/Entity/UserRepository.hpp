#pragma once

#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "../DBConnector.hpp"

struct User
{
	int id;
	std::string name;
	std::string password;
};

class UserRepository
{
private:
	DBConnector* m_dbc;

public:
	UserRepository(DBConnector* dbc);
	~UserRepository();

	bool CreateUser(const User& user) const;

	std::vector<std::optional<User>> GetUsers() const;
	std::optional<User> GetById(int id) const;
	std::optional<User> GetByName(const std::string& name) const;
};
