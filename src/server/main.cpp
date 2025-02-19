#include <string>
#include <optional>
#include <vector>
#include "Entity/UserRepository.hpp"

int main() {
  UserRepository R;

  User U;
  R.get_by_id(1);

  return 0;

}