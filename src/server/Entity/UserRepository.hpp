#pragma once

struct User {
  int id;
  std::string name;
  std::string password;
};

class UserRepository {
public:
  // TODO:: connection
  UserRepository() {}

  bool create_user(User user) {};

  std::vector<std::optional<User>> get_users();
  std::optional<User> get_by_id(int id) {}
  std::optional<User> get_by_name(std::string name) {}
};