/**
 * @file JsonParser.hpp
 * @brief Header file for JSON parsing utilities.
 *
 * This header declares the JSON class, which encapsulates JSON values of various types,
 * and the JSONParser class, which provides functionality to parse JSON files.
 */
#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @class JSON
 * @brief Represents a JSON value supporting null, boolean, number, string, array, and object types.
 */
class JSON
{
public:
	/// @brief Alias for a JSON object (collection of key-value pairs).
	using Object = std::unordered_map<std::string, JSON>;
	/// @brief Alias for a JSON array (ordered list of JSON values).
	using Array = std::vector<JSON>;
	/// @brief Variant type holding all possible JSON value types.
	using Value = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

	/**
	 * @enum JSON::Type
	 * @brief Enumerates all possible JSON value types.
	 */
	enum class Type
	{
		NUL,	///< Represents a null value.
		BOOL,	///< Represents a boolean value (true/false).
		NUMBER, ///< Represents a numeric value (stored as double).
		STRING, ///< Represents a string value.
		ARRAY,	///< Represents an array of JSON values.
		OBJECT	///< Represents an object (key-value pairs).
	};

public:
	/**
	 * @brief Constructs a null JSON value.
	 */
	explicit JSON();

	/**
	 * @brief Constructs a boolean JSON value.
	 * @param b Boolean value to store.
	 */
	explicit JSON(bool b);

	/**
	 * @brief Constructs a numeric JSON value.
	 * @param num Numeric value to store.
	 */
	explicit JSON(double num);

	/**
	 * @brief Constructs a string JSON value.
	 * @param s String value to store.
	 */
	explicit JSON(const std::string& s);

	/**
	 * @brief Constructs a string JSON value from C-style string.
	 * @param s Null-terminated character string to store.
	 */
	explicit JSON(const char* s);

	/**
	 * @brief Constructs a JSON array from existing array.
	 * @param arr Array of JSON values to store.
	 */
	explicit JSON(const Array& arr);

	/**
	 * @brief Constructs a JSON object from existing object.
	 * @param obj Object (key-value pairs) to store.
	 */
	explicit JSON(const Object& obj);

	/**
	 * @brief Constructs a JSON array by moving existing array.
	 * @param arr Array of JSON values to move.
	 */
	explicit JSON(Array&& arr);

	/**
	 * @brief Constructs a JSON object by moving existing object.
	 * @param obj Object (key-value pairs) to move.
	 */
	explicit JSON(Object&& obj);

	/**
	 * @brief Gets the type of the JSON value.
	 * @return Type enum indicating current value's type.
	 */
	Type GetType() const;

	/**
	 * @brief Retrieves boolean value stored in JSON.
	 * @return Stored boolean value.
	 * @throw std::runtime_error If JSON is not a boolean.
	 */
	bool AsBool() const;

	/**
	 * @brief Retrieves numeric value stored in JSON.
	 * @return Stored numeric value as double.
	 * @throw std::runtime_error If JSON is not a number.
	 */
	double AsNumber() const;

	/**
	 * @brief Retrieves string value stored in JSON.
	 * @return Reference to stored string value.
	 * @throw std::runtime_error If JSON is not a string.
	 */
	const std::string& AsString() const;

	/**
	 * @brief Retrieves array stored in JSON.
	 * @return Reference to stored array.
	 * @throw std::runtime_error If JSON is not an array.
	 */
	const Array& AsArray() const;

	/**
	 * @brief Retrieves object stored in JSON.
	 * @return Reference to stored object.
	 * @throw std::runtime_error If JSON is not an object.
	 */
	const Object& AsObject() const;

	/**
	 * @brief Provides direct access to the underlying variant value.
	 * @return Const reference to the stored variant value.
	 */
	const Value& GetValue() const;

	/**
	 * @brief Accesses object member by key.
	 * @param key Key of the object member to access.
	 * @return Reference to JSON value associated with key.
	 * @throw std::runtime_error If JSON is not an object or key not found.
	 */
	const JSON& operator[](const std::string& key) const;

private:
	Value m_value; ///< Underlying storage for JSON value.
};

/**
 * @class JSONParser
 * @brief Parses JSON content from files and constructs JSON object hierarchy.
 */
class JSONParser
{
public:
	/**
	 * @brief Constructs parser for specified JSON file.
	 * @param file_path Path to JSON file to parse.
	 *
	 * The file is read completely into an internal buffer prior to parsing.
	 *
	 * @throw std::runtime_error If file cannot be opened.
	 */
	explicit JSONParser(const std::filesystem::path& file_path);

	/**
	 * @brief Parses entire JSON content from input.
	 * @return Root JSON value of parsed content.
	 *
	 * This function starts the parsing process by calling ParseValue() and ensures that
	 * all characters are consumed. If there are extra trailing characters, an exception is thrown.
	 *
	 * @throw std::runtime_error If any syntax error is encountered or if there are unexpected trailing characters.
	 */
	JSON Parse();

private:
	/**
	 * @brief Skips all whitespace characters at current position.
	 */
	void SkipWhitespace();

	/**
	 * @brief Returns current character without advancing position.
	 * @return Current character or '\0' at end of input.
	 */
	char Peek() const;

	/**
	 * @brief Consumes and returns current character.
	 * @return Current character.
	 * @throw std::runtime_error If at end of input.
	 */
	char Get();

	/**
	 * @brief Parses JSON value based on current character.
	 * @return Parsed JSON value.
	 * @throw std::runtime_error For unexpected characters.
	 */
	JSON ParseValue();

	/**
	 * @brief Parses JSON string value.
	 * @return JSON string value.
	 * @throw std::runtime_error For invalid escape sequences or quotes.
	 */
	JSON ParseString();

	/**
	 * @brief Parses JSON numeric value.
	 * @return JSON numeric value.
	 * @throw std::runtime_error For invalid numeric format.
	 */
	JSON ParseNumber();

	/**
	 * @brief Parses JSON boolean value (true/false).
	 * @return JSON boolean value.
	 * @throw std::runtime_error For invalid boolean literals.
	 */
	JSON ParseBool();

	/**
	 * @brief Parses JSON null value.
	 * @return JSON null value.
	 * @throw std::runtime_error For invalid null literal.
	 */
	JSON ParseNull();

	/**
	 * @brief Parses JSON array value.
	 * @return JSON array value.
	 * @throw std::runtime_error For syntax errors in array.
	 */
	JSON ParseArray();

	/**
	 * @brief Parses JSON object value.
	 * @return JSON object value.
	 * @throw std::runtime_error For syntax errors in object.
	 */
	JSON ParseObject();

	/**
	 * @brief Parses escape sequence in JSON string.
	 * @return String containing escaped character.
	 * @throw std::runtime_error For unsupported escape sequences.
	 */
	std::string ParseEscapeSequence();

private:
	std::string m_input; ///< Buffer holding JSON content to parse.
	size_t m_pos = 0;	 ///< Current parsing position in input buffer.
};
