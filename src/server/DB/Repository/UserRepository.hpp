#pragma once

#include <optional>
#include <vector>

#include "Connection/DBConnector.hpp"
#include "Entity/User.hpp"

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
