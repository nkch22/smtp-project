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
	os << j.Serialize(false); // Output in compact form.
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
std::string JSON::Serialize(bool pretty, int indent) const
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
		// Use the helper to serialize an array.
		return SerializeContainer(AsArray(), pretty, indent, "[", "]", [](const JSON& elem, int newIndent, bool pretty)
								  { return elem.Serialize(pretty, newIndent); });

	case Type::OBJECT:
		// Use the helper to serialize an object.
		return SerializeContainer(AsObject(), pretty, indent, "{", "}",
								  [](const std::pair<const std::string, JSON>& p, int newIndent, bool pretty)
								  {
									  std::string res = "\"" + EscapeString(p.first) + "\":";
									  if (pretty) res += " ";
									  res += p.second.Serialize(pretty, newIndent);
									  return res;
								  });

	default:
		throw std::runtime_error("Unknown JSON type in serialization");
	}
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
} // namespace ISXJson
