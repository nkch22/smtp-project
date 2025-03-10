#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "../DBConnector.hpp"

struct User {
  int id;
  std::string name;
  std::string password;
};

class UserRepository {
private:
  DBConnector* dbc;
public:
  // UserRepository();
  UserRepository(DBConnector* dbc);
  ~UserRepository();

  bool create_user(User user);

  std::vector<std::optional<User>> get_users();
  std::optional<User> get_by_id(int id);
  std::optional<User> get_by_name(std::string name);
};