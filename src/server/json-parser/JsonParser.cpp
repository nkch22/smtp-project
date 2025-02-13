#include "JsonParser.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

JSON::JSON()
    : value_(nullptr) {
}

JSON::JSON(bool b)
    : value_(b) {
}

JSON::JSON(double num)
    : value_(num) {
}

JSON::JSON(const std::string& s)
    : value_(s) {
}

JSON::JSON(const char* s)
    : value_(std::string(s)) {
}

JSON::JSON(const Array& arr)
    : value_(arr) {
}

JSON::JSON(const Object& obj)
    : value_(obj) {
}

JSON::Type JSON::GetType() const {
  if (std::holds_alternative<std::nullptr_t>(value_)) {
    return Type::Null;
  } else if (std::holds_alternative<bool>(value_)) {
    return Type::Bool;
  } else if (std::holds_alternative<double>(value_)) {
    return Type::Number;
  } else if (std::holds_alternative<std::string>(value_)) {
    return Type::String;
  } else if (std::holds_alternative<Array>(value_)) {
    return Type::Array;
  } else if (std::holds_alternative<Object>(value_)) {
    return Type::Object;
  }
  throw std::runtime_error("Invalid JSON type encountered");
}

bool JSON::AsBool() const {
  if (GetType() != Type::Bool) {
    throw std::runtime_error("Not a boolean");
  }
  return std::get<bool>(value_);
}

double JSON::AsNumber() const {
  if (GetType() != Type::Number) {
    throw std::runtime_error("Not a number");
  }
  return std::get<double>(value_);
}

const std::string& JSON::AsString() const {
  if (GetType() != Type::String) {
    throw std::runtime_error("Not a string");
  }
  return std::get<std::string>(value_);
}

const JSON::Array& JSON::AsArray() const {
  if (GetType() != Type::Array) {
    throw std::runtime_error("Not an array");
  }
  return std::get<Array>(value_);
}

const JSON::Object& JSON::AsObject() const {
  if (GetType() != Type::Object) {
    throw std::runtime_error("Not an object");
  }
  return std::get<Object>(value_);
}

const JSON::Value& JSON::GetValue() const {
  return value_;
}

JSONParser::JSONParser(const std::string& input)
    : input_(input) {
}

void JSONParser::SkipWhitespace() {
  while (pos_ < input_.size() &&
         (std::isspace(static_cast<unsigned char>(input_[pos_])) != 0)) {
    pos_++;
  }
}

char JSONParser::Peek() const {
  return pos_ < input_.size() ? input_[pos_] : '\0';
}

char JSONParser::Get() {
  if (pos_ >= input_.size()) {
    throw std::runtime_error("Unexpected end of input at position " +
                             std::to_string(pos_));
  }
  return input_[pos_++];
}

JSON JSONParser::ParseValue() {
  SkipWhitespace();

  char c = Peek();
  switch (c) {
    case '"':
      return ParseString();
    case '{':
      return ParseObject();
    case '[':
      return ParseArray();
    case 't':
    case 'f':
      return ParseBool();
    case 'n':
      return ParseNull();
    default:
      if (c == '-' || (std::isdigit(static_cast<unsigned char>(c)) != 0)) {
        return ParseNumber();
      }
      throw std::runtime_error("Unexpected character: " + std::string(1, c));
  }
}

JSON JSONParser::ParseString() {
  std::string value;
  if (Get() != '"') {
    throw std::runtime_error("Expected opening quote");
  }

  while (Peek() != '"') {
    if (Peek() == '\\') {
      Get();  // backslash
      value += ParseEscapeSequence();
    } else {
      value += Get();
    }
  }
  Get();  // closing quote
  return JSON(value);
}

JSON JSONParser::ParseNumber() {
  size_t start = pos_;
  if (Peek() == '-') {
    Get();
  }

  while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
    Get();
  }

  if (Peek() == '.') {
    Get();
    while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
      Get();
    }
  }
  if (Peek() == 'e' || Peek() == 'E') {
    Get();

    if (Peek() == '+' || Peek() == '-') {
      Get();
    }

    while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
      Get();
    }
  }

  return JSON(std::stod(input_.substr(start, pos_ - start)));
}

JSON JSONParser::ParseBool() {
  if (input_.compare(pos_, 4, "true") == 0) {
    pos_ += 4;
    return JSON(true);
  }

  if (input_.compare(pos_, 5, "false") == 0) {
    pos_ += 5;
    return JSON(false);
  }
  throw std::runtime_error("Invalid boolean value");
}

JSON JSONParser::ParseNull() {
  if (input_.compare(pos_, 4, "null") == 0) {
    pos_ += 4;
    return JSON();
  }
  throw std::runtime_error("Invalid null value");
}

JSON JSONParser::ParseArray() {
  std::vector<JSON> elements;
  Get();  // '['

  while (Peek() != ']') {
    elements.push_back(ParseValue());
    SkipWhitespace();

    if (Peek() == ',') {
      Get();
    }
  }
  Get();  // ']'
  return JSON(std::move(elements));
}

JSON JSONParser::ParseObject() {
  JSON::Object obj;
  Get();  // '{'
  SkipWhitespace();

  while (Peek() != '}') {
    SkipWhitespace();
    auto key = ParseString().AsString();
    SkipWhitespace();

    if (Get() != ':') {
      throw std::runtime_error("Expected colon");
    }

    SkipWhitespace();
    obj.emplace(std::move(key), ParseValue());
    SkipWhitespace();

    if (Peek() == ',') {
      Get();
      SkipWhitespace();
    }
  }
  Get();  // '}'
  return JSON(obj);
}

std::string JSONParser::ParseEscapeSequence() {
  char esc = Get();
  switch (esc) {
    case '"': {
      return "\"";
    }
    case '\\': {
      return "\\";
    }
    case '/': {
      return "/";
    }
    case 'b': {
      return "\b";
    }
    case 'f': {
      return "\f";
    }
    case 'n': {
      return "\n";
    }
    case 'r': {
      return "\r";
    }
    case 't': {
      return "\t";
    }
    case 'u': {
      throw std::runtime_error("Unicode escape sequences are not supported");
    }
    default: {
      throw std::runtime_error("Invalid escape character in string");
    }
  }
}

JSON JSONParser::Parse() {
  JSON result = ParseValue();
  SkipWhitespace();

  if (pos_ != input_.size()) {
    throw std::runtime_error("Unexpected trailing characters");
  }
  return result;
}

void JSONParser::PrintJSON(const JSON& value, int indent) const {
  std::string indent_str(indent, ' ');

  switch (value.GetType()) {
    case JSON::Type::Null: {
      std::cout << "null";
      break;
    }
    case JSON::Type::Bool: {
      std::cout << (value.AsBool() ? "true" : "false");
      break;
    }
    case JSON::Type::Number: {
      std::cout << value.AsNumber();
      break;
    }
    case JSON::Type::String: {
      std::cout << "\"" << value.AsString() << "\"";
      break;
    }
    case JSON::Type::Array: {
      std::cout << "[\n";
      const auto& arr = value.AsArray();

      for (size_t i = 0; i < arr.size(); i++) {
        std::cout << indent_str << "  ";
        PrintJSON(arr[i], indent + 2);

        if (i != arr.size() - 1) {
          std::cout << ",";
        }
        std::cout << "\n";
      }
      std::cout << indent_str << "]";
      break;
    }
    case JSON::Type::Object: {
      std::cout << "{\n";

      const auto& obj = value.AsObject();
      bool first = true;

      for (const auto& pair : obj) {
        if (!first) {
          std::cout << ",\n";
        }
        std::cout << indent_str << "  \"" << pair.first << "\": ";
        PrintJSON(pair.second, indent + 2);
        first = false;
      }
      std::cout << "\n" << indent_str << "}";
      break;
    }
    default: {
      throw std::runtime_error("Unknown JSON type encountered");
    }
  }
}
