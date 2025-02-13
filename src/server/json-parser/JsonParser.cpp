#include "JsonParser.hpp"

JSON::JSON()
    : type(Type::Null) {
}

JSON::JSON(bool b)
    : type(Type::Bool),
      bool_value(b) {
}

JSON::JSON(double num)
    : type(Type::Number),
      number_value(num) {
}

JSON::JSON(const std::string& s)
    : type(Type::String),
      string_value(s) {
}

JSON::JSON(const std::vector<JSON>& arr)
    : type(Type::Array),
      array_value(arr) {
}

JSON::JSON(const std::unordered_map<std::string, JSON>& obj)
    : type(Type::Object),
      object_value(obj) {
}
