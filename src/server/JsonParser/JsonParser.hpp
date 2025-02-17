#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class JSON
{
public:
	using Object = std::unordered_map<std::string, JSON>;
	using Array = std::vector<JSON>;
	using Value = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

	enum class Type
	{
		NUL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT
	};

public:
	explicit JSON();
	explicit JSON(bool b);
	explicit JSON(double num);
	explicit JSON(const std::string& s);
	explicit JSON(const char* s);
	explicit JSON(const Array& arr);
	explicit JSON(const Object& obj);
	explicit JSON(Array&& arr);
	explicit JSON(Object&& obj);

public:
	Type GetType() const;

	bool AsBool() const;
	double AsNumber() const;
	const std::string& AsString() const;
	const Array& AsArray() const;
	const Object& AsObject() const;
	const Value& GetValue() const;

public:
	const JSON& operator[](const std::string& key) const;

private:
	Value m_value;
};

class JSONParser
{
public:
	explicit JSONParser(const std::filesystem::path& file_path);

	JSON Parse();

private:
	void SkipWhitespace();
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
	std::string m_input;
	size_t m_pos = 0;
};
