#pragma once

#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class JSON {
 public:
  enum class Type { Null, Bool, Number, String, Array, Object };

  Type type;

 public:
  bool bool_value;
  double number_value;
  std::string string_value;
  std::vector<JSON> array_value;
  std::unordered_map<std::string, JSON> object_value;

 public:
  explicit JSON();
  explicit JSON(bool b);
  explicit JSON(double num);
  explicit JSON(const std::string& s);
  explicit JSON(const std::vector<JSON>& arr);
  explicit JSON(const std::unordered_map<std::string, JSON>& obj);
};
