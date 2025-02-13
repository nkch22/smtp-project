#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class JSON {
 public:
  using Object = std::unordered_map<std::string, JSON>;
  using Array = std::vector<JSON>;
  using Value =
      std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

  enum class Type { Null, Bool, Number, String, Array, Object };

 public:
  explicit JSON();
  explicit JSON(bool b);
  explicit JSON(double num);
  explicit JSON(const std::string& s);
  explicit JSON(const char* s);
  explicit JSON(const Array& arr);
  explicit JSON(const Object& obj);

 public:
  Type GetType() const;

  bool AsBool() const;
  double AsNumber() const;
  const std::string& AsString() const;
  const Array& AsArray() const;
  const Object& AsObject() const;
  const Value& GetValue() const;

 private:
  Value value_;
};

class JSONParser {
 public:
  explicit JSONParser(const std::string& inp);

 public:
  JSON Parse();
  void PrintJSON(const JSON& value, int indent) const;

 private:
  void SkipWhitespace();
  void ConsumeDigits();
  char Peek() const;
  char Get();

 private:
  JSON ParseValue();
  JSON ParseString();
  JSON ParseNumber();
  JSON ParseBool();
  JSON ParseNull();
  JSON ParseArray();
  JSON ParseObject();
  std::string ParseEscapeSequence();

 private:
  std::string input_;
  size_t pos_ = 0;
};
