#include "JsonParser.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

JSON::JSON() : m_value(nullptr) {}

JSON::JSON(bool b) : m_value(b) {}

JSON::JSON(double num) : m_value(num) {}

JSON::JSON(const std::string& s) : m_value(s) {}

JSON::JSON(const char* s) : m_value(std::string(s)) {}

JSON::JSON(const Array& arr) : m_value(arr) {}
JSON::JSON(Array&& arr) : m_value(std::move(arr)) {}

JSON::JSON(const Object& obj) : m_value(obj) {}
JSON::JSON(Object&& obj) : m_value(std::move(obj)) {}

JSON::Type JSON::GetType() const
{
	switch (m_value.index())
	{
	case 0:
		return Type::NUL;
	case 1:
		return Type::BOOL;
	case 2:
		return Type::NUMBER;
	case 3:
		return Type::STRING;
	case 4:
		return Type::ARRAY;
	case 5:
		return Type::OBJECT;
	default:
		throw std::runtime_error("Invalid JSON type encountered");
	}
}

bool JSON::AsBool() const
{
	if (GetType() != Type::BOOL) throw std::runtime_error("Not a boolean");

	return std::get<bool>(m_value);
}

double JSON::AsNumber() const
{
	if (GetType() != Type::NUMBER) throw std::runtime_error("Not a number");

	return std::get<double>(m_value);
}

const std::string& JSON::AsString() const
{
	if (GetType() != Type::STRING) throw std::runtime_error("Not a string");

	return std::get<std::string>(m_value);
}

const JSON::Array& JSON::AsArray() const
{
	if (GetType() != Type::ARRAY) throw std::runtime_error("Not an array");

	return std::get<Array>(m_value);
}

const JSON::Object& JSON::AsObject() const
{
	if (GetType() != Type::OBJECT) throw std::runtime_error("Not an object");

	return std::get<Object>(m_value);
}

const JSON::Value& JSON::GetValue() const
{
	return m_value;
}

JSONParser::JSONParser(const std::string& file_path)
{
	std::ifstream file(file_path);
	if (!file) throw std::runtime_error("Could not open file: " + file_path);

	m_input = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	m_pos = 0;
}

void JSONParser::SkipWhitespace()
{
	while (m_pos < m_input.size() && (std::isspace(static_cast<unsigned char>(m_input[m_pos])) != 0)) m_pos++;
}

char JSONParser::Peek() const
{
	return m_pos < m_input.size() ? m_input[m_pos] : '\0';
}

char JSONParser::Get()
{
	if (m_pos >= m_input.size())
		throw std::runtime_error("Unexpected end of input at position " + std::to_string(m_pos));

	return m_input[m_pos++];
}

JSON JSONParser::ParseValue()
{
	SkipWhitespace();

	char c = Peek();
	switch (c)
	{
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
		if (c == '-' || (std::isdigit(static_cast<unsigned char>(c)) != 0)) return ParseNumber();
		throw std::runtime_error("Unexpected character: " + std::string(1, c));
	}
}

JSON JSONParser::ParseString()
{
	std::string value;

	if (Get() != '"') throw std::runtime_error("Expected opening quote");

	while (Peek() != '"')
	{
		if (Peek() == '\\')
		{
			Get(); // backslash
			value += ParseEscapeSequence();
		}
		else
		{
			value += Get();
		}
	}
	Get(); // closing quote
	return JSON(value);
}

JSON JSONParser::ParseNumber()
{
	size_t start = m_pos;

	if (Peek() == '-') Get();

	while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();

	if (Peek() == '.')
	{
		Get();
		while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();
	}
	if (Peek() == 'e' || Peek() == 'E')
	{
		Get();

		if (Peek() == '+' || Peek() == '-') Get();

		while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();
	}

	return JSON(std::stod(m_input.substr(start, m_pos - start)));
}

JSON JSONParser::ParseBool()
{
	if (m_input.compare(m_pos, 4, "true") == 0)
	{
		m_pos += 4;
		return JSON(true);
	}

	if (m_input.compare(m_pos, 5, "false") == 0)
	{
		m_pos += 5;
		return JSON(false);
	}
	throw std::runtime_error("Invalid boolean value");
}

JSON JSONParser::ParseNull()
{
	if (m_input.compare(m_pos, 4, "null") == 0)
	{
		m_pos += 4;
		return JSON();
	}
	throw std::runtime_error("Invalid null value");
}

JSON JSONParser::ParseArray()
{
	std::vector<JSON> elements;
	Get(); // '['

	while (Peek() != ']')
	{
		elements.push_back(ParseValue());
		SkipWhitespace();

		if (Peek() == ',') Get();
	}
	Get(); // ']'
	return JSON(std::move(elements));
}

JSON JSONParser::ParseObject()
{
	JSON::Object obj;
	Get(); // '{'
	SkipWhitespace();

	while (Peek() != '}')
	{
		SkipWhitespace();
		auto key = ParseString().AsString();
		SkipWhitespace();

		if (Get() != ':') throw std::runtime_error("Expected colon");

		SkipWhitespace();
		obj.emplace(std::move(key), ParseValue());
		SkipWhitespace();

		if (Peek() == ',')
		{
			Get();
			SkipWhitespace();
		}
	}
	Get(); // '}'
	return JSON(obj);
}

std::string JSONParser::ParseEscapeSequence()
{
	char esc = Get();
	switch (esc)
	{
	case '"':
		return "\"";
	case '\\':
		return "\\";
	case '/':
		return "/";
	case 'b':
		return "\b";
	case 'f':
		return "\f";
	case 'n':
		return "\n";
	case 'r':
		return "\r";
	case 't':
		return "\t";
	case 'u':
		throw std::runtime_error("Unicode escape sequences are not supported");
	default:
		throw std::runtime_error("Invalid escape character in string");
	}
}

JSON JSONParser::Parse()
{
	JSON result = ParseValue();
	SkipWhitespace();

	if (m_pos != m_input.size()) throw std::runtime_error("Unexpected trailing characters");

	return result;
}

const JSON& JSON::operator[](const std::string& key) const
{
	const Object& obj = AsObject();
	auto it = obj.find(key);
	if (it == obj.end()) throw std::runtime_error("Key not found: " + key);

	return it->second;
}
