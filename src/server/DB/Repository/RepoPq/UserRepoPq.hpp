#ifndef USERREPOPQ_HPP
#define USERREPOPQ_HPP

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "../../Entity/User.hpp"
#include "../Interface/UserRepo.hpp"

class UserRepoPq: public UserRepo {
private:
  pqxx::connection conn;

public:
  UserRepoPq(std::string connection_str);
  ~UserRepoPq() = default;

  bool CreateUser(User user);

  std::vector<std::optional<User>> GetUsers();
  std::optional<User> GetById(int id);
  std::optional<User> GetByName(std::string name);
};

#endif
