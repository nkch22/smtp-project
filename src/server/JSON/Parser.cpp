/**
 * @file Parser.cpp
 * @brief Implementation of the Parser class.
 *
 * This file implements the methods for parsing JSON content from a file.
 * It supports parsing of strings, numbers, booleans, null, arrays, and objects.
 */

#include "Parser.hpp"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace ISXJson
{
Parser::Parser(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file) throw std::runtime_error("Could not open file: " + file_path.string());

	m_input = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	m_pos = 0;
}

// New constructor that accepts a JSON string directly.
Parser::Parser(const std::string& input)
	: m_input(input), m_pos(0)
{
	// No further action required. The input is already in a string.
}

void Parser::SkipWhitespace()
{
	// Advance m_pos until a non-whitespace character is found.
	while (m_pos < m_input.size() && (std::isspace(static_cast<unsigned char>(m_input[m_pos])) != 0)) m_pos++;
}

char Parser::Peek() const
{
	// Return the current character without advancing, or '\0' if at the end.
	return m_pos < m_input.size() ? m_input[m_pos] : '\0';
}

char Parser::Get()
{
	// Ensure that we are not reading past the end of the input.
	if (m_pos >= m_input.size())
		throw std::runtime_error("Unexpected end of input at position " + std::to_string(m_pos));

	// Return the character at m_pos and then advance the pointer.
	return m_input[m_pos++];
}

JSON Parser::ParseValue()
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

JSON Parser::ParseString()
{
	char quote = Get(); // Consume opening quote
	if (quote != '"') throw std::runtime_error("Expected opening quote for string");

	std::string result;
	while (true)
	{
		if (m_pos >= m_input.size()) throw std::runtime_error("Unterminated string literal");
		char c = Get();
		if (c == '"') break;
		if (c == '\\')
		{
			if (m_pos >= m_input.size()) throw std::runtime_error("Unterminated escape sequence");
			char esc = Get();
			switch (esc)
			{
			case '"':
				result.push_back('\"');
				break;
			case '\\':
				result.push_back('\\');
				break;
			case '/':
				result.push_back('/');
				break;
			case 'b':
				result.push_back('\b');
				break;
			case 'f':
				result.push_back('\f');
				break;
			case 'n':
				result.push_back('\n');
				break;
			case 'r':
				result.push_back('\r');
				break;
			case 't':
				result.push_back('\t');
				break;
			default:
				throw std::runtime_error("Unsupported escape sequence");
			}
		}
		else
		{
			result.push_back(c);
		}
	}
	return { result };
}

JSON Parser::ParseNumber()
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
	return { std::stod(m_input.substr(start, m_pos - start)) };
}

JSON Parser::ParseBool()
{
	// Check for "true" literal.
	if (m_input.compare(m_pos, 4, "true") == 0)
	{
		m_pos += 4;
		return { true };
	}

	// Check for "false" literal.
	if (m_input.compare(m_pos, 5, "false") == 0)
	{
		m_pos += 5;
		return { false };
	}
	throw std::runtime_error("Invalid boolean value");
}

JSON Parser::ParseNull()
{
	// Check for "null" literal.
	if (m_input.compare(m_pos, 4, "null") == 0)
	{
		m_pos += 4;
		return {};
	}
	throw std::runtime_error("Invalid null literal");
}

JSON Parser::ParseArray()
{
	std::vector<JSON> elements;
	Get(); // Consume '['
	SkipWhitespace();

	// Parse array elements until closing ']' is encountered.
	while (Peek() != ']')
	{
		elements.push_back(ParseValue());
		SkipWhitespace();

		// If a comma is present, consume it.
		if (Peek() == ',') Get();
	}
	Get(); // Consume closing ']'
	return { std::move(elements) };
}

JSON Parser::ParseObject()
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
	return { obj };
}

JSON Parser::Parse()
{
	// Parse the main JSON value.
	JSON result = ParseValue();
	SkipWhitespace();

	// Check that no extra characters remain after the JSON value.
	if (m_pos != m_input.size()) throw std::runtime_error("Unexpected trailing characters");

	return result;
}

} // namespace ISXJson
