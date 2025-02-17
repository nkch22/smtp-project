/**
 * @file JsonParser.cpp
 * @brief Implements JSON parsing and JSON value handling.
 *
 * This file contains the implementation of the JSON and JSONParser classes as declared
 * in JsonParser.hpp. It includes routines for parsing strings, numbers, booleans, arrays,
 * and objects from JSON input.
 */
#include "JsonParser.hpp"

#include <cctype>
#include <filesystem>
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

JSONParser::JSONParser(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file) throw std::runtime_error("Could not open file: " + file_path.string());

	m_input = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	m_pos = 0;
}

void JSONParser::SkipWhitespace()
{
	// Advance m_pos until a non-whitespace character is found.
	while (m_pos < m_input.size() && (std::isspace(static_cast<unsigned char>(m_input[m_pos])) != 0)) m_pos++;
}

char JSONParser::Peek() const
{
	// Return the current character without advancing, or '\0' if at the end.
	return m_pos < m_input.size() ? m_input[m_pos] : '\0';
}

char JSONParser::Get()
{
	// Ensure that we are not reading past the end of the input.
	if (m_pos >= m_input.size())
		throw std::runtime_error("Unexpected end of input at position " + std::to_string(m_pos));

	// Return the character at m_pos and then advance the pointer.
	return m_input[m_pos++];
}

JSON JSONParser::ParseValue()
{
	// Skip any whitespace before parsing the JSON value.
	SkipWhitespace();

	char c = Peek();
	// Determine type of JSON value based on the current character.
	switch (c)
	{
	case '"': // String value.
		return ParseString();
	case '{': // Object value.
		return ParseObject();
	case '[': // Array value.
		return ParseArray();
	case 't': // Boolean true.
	case 'f': // Boolean false.
		return ParseBool();
	case 'n': // Null value.
		return ParseNull();
	default:
		// If number or negative sign encountered, parse number.
		if (c == '-' || (std::isdigit(static_cast<unsigned char>(c)) != 0)) return ParseNumber();
		throw std::runtime_error("Unexpected character: " + std::string(1, c));
	}
}

JSON JSONParser::ParseString()
{
	std::string value;

	// Expect an opening quote character.
	if (Get() != '"') throw std::runtime_error("Expected opening quote");

	// Read characters until the closing quote is encountered.
	while (Peek() != '"')
	{
		if (Peek() == '\\')
		{
			Get(); // Consume the backslash.
			// Parse escape sequence following backslash.
			value += ParseEscapeSequence();
		}
		else
		{
			// Append the current character to the string.
			value += Get();
		}
	}
	Get(); // Consume closing quote.
	return JSON(value);
}

JSON JSONParser::ParseNumber()
{
	// Record the starting position of the number.
	size_t start = m_pos;

	// Check for a negative sign.
	if (Peek() == '-') Get();

	// Process integer digits.
	while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();

	// Process fractional part if present.
	if (Peek() == '.')
	{
		Get(); // Consume the dot.
		while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();
	}

	// Process exponent part if present.
	if (Peek() == 'e' || Peek() == 'E')
	{
		Get(); // Consume 'e' or 'E'.

		// Consume optional '+' or '-' sign.
		if (Peek() == '+' || Peek() == '-') Get();

		// Process exponent digits.
		while (std::isdigit(static_cast<unsigned char>(Peek())) != 0) Get();
	}

	// Convert accumulated substring to a double.
	return JSON(std::stod(m_input.substr(start, m_pos - start)));
}

JSON JSONParser::ParseBool()
{
	// Check for "true" literal.
	if (m_input.compare(m_pos, 4, "true") == 0)
	{
		m_pos += 4;
		return JSON(true);
	}

	// Check for "false" literal.
	if (m_input.compare(m_pos, 5, "false") == 0)
	{
		m_pos += 5;
		return JSON(false);
	}
	throw std::runtime_error("Invalid boolean value");
}

JSON JSONParser::ParseNull()
{
	// Check for "null" literal.
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
	Get(); // Consume '['

	// Parse array elements until closing ']' is encountered.
	while (Peek() != ']')
	{
		elements.push_back(ParseValue());
		SkipWhitespace();

		// If a comma is present, consume it.
		if (Peek() == ',') Get();
	}
	Get(); // Consume closing ']'
	return JSON(std::move(elements));
}

JSON JSONParser::ParseObject()
{
	JSON::Object obj;
	Get(); // Consume '{'
	SkipWhitespace();

	// Parse key-value pairs until closing '}' is encountered.
	while (Peek() != '}')
	{
		SkipWhitespace();
		// Parse the key as a JSON string.
		auto key = ParseString().AsString();
		SkipWhitespace();

		// Expect a colon separating key and value.
		if (Get() != ':') throw std::runtime_error("Expected colon");

		SkipWhitespace();
		// Associate the parsed value with the key.
		obj.emplace(std::move(key), ParseValue());
		SkipWhitespace();

		// If a comma is found, consume it.
		if (Peek() == ',')
		{
			Get();
			SkipWhitespace();
		}
	}
	Get(); // Consume closing '}'
	return JSON(obj);
}

std::string JSONParser::ParseEscapeSequence()
{
	char esc = Get();
	// Return corresponding unescaped character based on the escape sequence.
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
		// Unicode escape sequences are not supported.
		throw std::runtime_error("Unicode escape sequences are not supported");
	default:
		throw std::runtime_error("Invalid escape character in string");
	}
}

JSON JSONParser::Parse()
{
	// Parse the main JSON value.
	JSON result = ParseValue();
	SkipWhitespace();

	// Check that no extra characters remain after the JSON value.
	if (m_pos != m_input.size()) throw std::runtime_error("Unexpected trailing characters");

	return result;
}

const JSON& JSON::operator[](const std::string& key) const
{
	// Retrieve the underlying object.
	const Object& obj = AsObject();
	auto it = obj.find(key);
	if (it == obj.end()) throw std::runtime_error("Key not found: " + key);

	return it->second;
}
