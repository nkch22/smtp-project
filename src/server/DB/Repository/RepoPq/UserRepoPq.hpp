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

  bool create_user(User user);

  std::vector<std::optional<User>> get_users();
  std::optional<User> get_by_id(int id);
  std::optional<User> get_by_name(std::string name);
};

#endif
