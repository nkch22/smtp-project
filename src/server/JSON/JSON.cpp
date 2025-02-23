/**
 * @file JSON.cpp
 * @brief Implementation of the JSON class.
 *
 * This file implements the JSON class for representing and manipulating JSON values.
 * It provides constructors for various types, accessors, operator overloads, and
 * functionality to serialize the JSON value into a string.
 */

#include "JSON.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "Parser.hpp" // Ensure that Parser is available.

namespace ISXJson
{

JSON::JSON() : m_value{nullptr} {}						   // Default: null
JSON::JSON(bool b) : m_value{b} {}						   // Boolean
JSON::JSON(double num) : m_value{num} {}				   // Number (double)
JSON::JSON(int num) : m_value{static_cast<double>(num)} {} // Convert int to double
JSON::JSON(const std::string& s) : m_value{s} {}		   // String from std::string
JSON::JSON(const char* s) : m_value{std::string(s)} {}	   // String from C-string
JSON::JSON(const Array& arr) : m_value{arr} {}			   // Array copy
JSON::JSON(Array&& arr) : m_value{std::move(arr)} {}	   // Array move
JSON::JSON(const Object& obj) : m_value{obj} {}			   // Object copy
JSON::JSON(Object&& obj) : m_value{std::move(obj)} {}	   // Object move

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

const JSON& JSON::operator[](const std::string& key) const
{
	const auto& obj = AsObject();
	auto it = obj.find(key);
	if (it == obj.end()) throw std::runtime_error("Key not found: " + key);
	return it->second;
}

JSON& JSON::operator[](const std::string& key)
{
	if (GetType() != Type::OBJECT) throw std::runtime_error("Not an object");
	auto& obj = std::get<Object>(m_value);
	return obj[key];
}

const JSON& JSON::operator[](std::size_t index) const
{
	const Array& arr = AsArray();
	if (index >= arr.size()) throw std::runtime_error("Index out of range");
	return arr[index];
}

JSON& JSON::operator[](std::size_t index)
{
	auto& arr = const_cast<Array&>(AsArray());
	if (index >= arr.size()) throw std::runtime_error("Index out of range");
	return arr[index];
}

std::ostream& operator<<(std::ostream& os, const JSON& j)
{
	os << j.dump(); // Output using dump (defaults to compact output).
	return os;
}

std::size_t JSON::Size() const
{
	if (GetType() == Type::ARRAY) return std::get<Array>(m_value).size();
	if (GetType() == Type::OBJECT) return std::get<Object>(m_value).size();
	throw std::runtime_error("Size is only applicable to arrays and objects");
}

bool JSON::Contains(const std::string& key) const
{
	if (GetType() != Type::OBJECT) throw std::runtime_error("Not an object");
	const auto& obj = std::get<Object>(m_value);
	return obj.find(key) != obj.end();
}

// Anonymous namespace for helper function used by SerializeArray and SerializeObject.
namespace
{
// SerializeContainer formats a container (array or object) into a JSON string.
// 'container' is the collection to serialize,
// 'open' and 'close' are the opening and closing delimiters,
// 'formatter' is a lambda that converts each element to a string.
template<typename Container, typename Formatter>
std::string SerializeContainer(const Container& container, bool pretty, int indent, const std::string& open,
							   const std::string& close, Formatter formatter)
{
	std::string indent_str(indent, ' ');
	std::string result = open; // Add opening delimiter.

	if (pretty) result += "\n";
	size_t count = 0;
	// Iterate through each element in the container.
	for (const auto& elem : container)
	{
		if (pretty) result += std::string(indent + 2, ' '); // Extra indent for elements.
		result += formatter(elem, indent + 2, pretty);		// Format the element.

		if (++count != container.size()) result += ","; // Separate elements with a comma.
		if (pretty) result += "\n";
	}
	if (pretty) result += indent_str; // Matching indent for closing delimiter.
	result += close;				  // Add closing delimiter.
	return result;
}
} // namespace

// Serialize converts the JSON value into its JSON-formatted string.
std::string JSON::Serialize(bool pretty, int indent, char indent_char) const
{
	switch (GetType())
	{
	case Type::NUL:
		return "null";

	case Type::BOOL:
		return AsBool() ? "true" : "false";

	case Type::NUMBER:
	{
		double num = AsNumber();
		double intpart;
		std::ostringstream oss;
		// Check if the number has no fractional part.
		if (std::modf(num, &intpart) == 0.0)
			oss << static_cast<long long>(intpart);
		else
			oss << num;
		return oss.str();
	}

	case Type::STRING:
		// Enclose the escaped string in double quotes.
		return "\"" + EscapeString(AsString()) + "\"";

	case Type::ARRAY:
		// Call the helper to serialize an array.
		return SerializeArray(pretty, indent, indent_char);

	case Type::OBJECT:
		// Call the helper to serialize an object.
		return SerializeObject(pretty, indent, indent_char);

	default:
		throw std::runtime_error("Unknown JSON type in serialization");
	}
}

// -----------------------------------------------------------------------------
// New helper function to serialize a JSON array.
std::string JSON::SerializeArray(bool pretty, int indent, char indent_char) const
{
	const Array& array = AsArray();
	std::ostringstream oss;
	oss << "[";
	if (pretty && !array.empty())
	{
		oss << "\n";
	}
	for (std::size_t i = 0; i < array.size(); ++i)
	{
		if (pretty)
		{
			oss << std::string(indent + 2, indent_char);
		}
		oss << array[i].Serialize(pretty, indent + 2, indent_char);
		if (i != array.size() - 1)
		{
			oss << ",";
		}
		if (pretty)
		{
			oss << "\n";
		}
	}
	if (pretty && !array.empty())
	{
		oss << std::string(indent, indent_char);
	}
	oss << "]";
	return oss.str();
}

// -----------------------------------------------------------------------------
// New helper function to serialize a JSON object.
std::string JSON::SerializeObject(bool pretty, int indent, char indent_char) const
{
	const Object& object = AsObject();
	std::ostringstream oss;
	oss << "{";
	if (pretty && !object.empty())
	{
		oss << "\n";
	}
	std::size_t count = 0;
	for (const auto& pair : object)
	{
		if (pretty)
		{
			oss << std::string(indent + 2, indent_char);
		}
		oss << "\"" << EscapeString(pair.first) << "\":";
		if (pretty)
		{
			oss << " ";
		}
		oss << pair.second.Serialize(pretty, indent + 2, indent_char);
		if (count != object.size() - 1)
		{
			oss << ",";
		}
		if (pretty)
		{
			oss << "\n";
		}
		++count;
	}
	if (pretty && !object.empty())
	{
		oss << std::string(indent, indent_char);
	}
	oss << "}";
	return oss.str();
}

std::string JSON::EscapeString(const std::string& input)
{
	// Define a local lambda to escape a single character.
	auto escape_char = [](char c) -> std::string
	{
		switch (c)
		{
		case '\"':
			return "\\\"";
		case '\\':
			return "\\\\";
		case '\b':
			return "\\b";
		case '\f':
			return "\\f";
		case '\n':
			return "\\n";
		case '\r':
			return "\\r";
		case '\t':
			return "\\t";
		default:
			if (static_cast<unsigned char>(c) < 0x20 || static_cast<unsigned char>(c) > 0x7E)
			{
				std::ostringstream oss;
				oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
				return oss.str();
			}
			else
			{
				return {c};
			}
		}
	};

	std::string output;
	for (char c : input) output += escape_char(c); // Use the lambda to get the escaped character.

	return output;
}

std::string JSON::dump(int indent, char indent_char) const
{
	if (indent < 0)
		return Serialize(false, 0, indent_char); // Compact output.
	else
		return Serialize(true, 0, indent_char); // Pretty printed output.
}

JSON JSON::parse(const std::string& input)
{
	// Create a parser from a string (using a new constructor that accepts string input).
	Parser parser(input);
	return parser.Parse();
}

std::istream& operator>>(std::istream& is, JSON& j)
{
	try
	{
		// Read the entire input stream into a string.
		std::string input((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		j = JSON::parse(input);
	}
	catch (const std::exception& ex)
	{
		// If parsing fails, set the stream failbit.
		is.setstate(std::ios::failbit);
	}
	return is;
}
} // namespace ISXJson
