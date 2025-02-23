/**
 * @file JSON.hpp
 * @brief Declaration of the JSON class for representing JSON values.
 *
 * This header file declares the JSON class which encapsulates JSON values
 * of various types including null, boolean, number, string, array, and object.
 * It also provides methods for type-safe access, modification, and serialization
 * of the value to a JSON-formatted string.
 */

#pragma once

#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ISXJson
{

/**
 * @class JSON
 * @brief Represents a JSON value.
 *
 * The JSON class provides a flexible container for storing JSON values. It supports
 * the following types:
 *  - Null: represents a null value.
 *  - Boolean: stores a true/false value.
 *  - Number: stores numeric values as double.
 *  - String: stores a string value.
 *  - Array: stores an ordered list of JSON values.
 *  - Object: stores a collection of key-value pairs where keys are strings.
 *
 * Internally, the class uses a std::variant to hold any one of these types and
 * provides type-safe accessors that throw exceptions if an incorrect type is accessed.
 *
 * @details The design of this class is influenced by the need for a flexible yet
 * type-safe container for JSON data. Instead of using a more dynamic type such as
 * a union, std::variant was chosen to ensure that all type conversions are checked
 * at runtime and that any mismatches produce a clear error message.
 */
class JSON
{
public:
	/// Alias for a JSON object (a set of key-value pairs).
	using Object = std::unordered_map<std::string, JSON>;
	/// Alias for a JSON array (an ordered list of JSON values).
	using Array = std::vector<JSON>;
	/// Variant type representing any JSON value.
	using Value = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

	/**
	 * @enum JSON::Type
	 * @brief Enumerates all possible JSON value types.
	 */
	enum class Type
	{
		NUL,	///< Null value.
		BOOL,	///< Boolean value.
		NUMBER, ///< Numeric value (stored as double).
		STRING, ///< String value.
		ARRAY,	///< Array of JSON values.
		OBJECT	///< Object of key-value pairs.
	};

public:
	/**
	 * @brief Default constructor that creates a null JSON value.
	 *
	 * @details This is the default state of a JSON without a defined value.
	 */
	JSON();

	/**
	 * @brief Constructs a JSON value storing a boolean.
	 * @param b Boolean value to store.
	 *
	 * @details This constructor allows you to implicitly create JSON boolean values.
	 */
	JSON(bool b);

	/**
	 * @brief Constructs a JSON value storing a number.
	 * @param num Numeric value to store (as double).
	 *
	 * @details Numeric values in JSON are always stored as doubles for simplicity.
	 */
	JSON(double num);

	/**
	 * @brief Constructs a JSON value storing a number from an integer.
	 * @param num Integer value to be converted and stored as double.
	 *
	 * @details This overload resolves the ambiguity between boolean and double
	 * conversions when using integer literals.
	 */
	JSON(int num);

	/**
	 * @brief Constructs a JSON value storing a string.
	 * @param s String value to store.
	 *
	 * @details This constructor is used when a std::string is available and allows
	 * the JSON value to hold textual data.
	 */
	JSON(const std::string& s);

	/**
	 * @brief Constructs a JSON value storing a string from a C-style string.
	 * @param s Null-terminated C-string.
	 *
	 * @details This overload enables the user to pass string literals directly
	 * without constructing a std::string explicitly.
	 */
	JSON(const char* s);

	/**
	 * @brief Constructs a JSON value storing an array (copy).
	 * @param arr Array of JSON values (copied into the new value).
	 *
	 * @details Useful when creating a JSON array from an initializer list or when
	 * the array is already available.
	 */
	JSON(const Array& arr);

	/**
	 * @brief Constructs a JSON value storing an array (move).
	 * @param arr Array of JSON values to move into the new value.
	 *
	 * @details This saves an unnecessary copy if the array is temporary.
	 */
	JSON(Array&& arr);

	/**
	 * @brief Constructs a JSON value storing an object (copy).
	 * @param obj Object of key-value pairs (copied into the new value).
	 *
	 * @details Allows construction of a JSON value representing an object using
	 * an existing map of key-value pairs.
	 */
	JSON(const Object& obj);

	/**
	 * @brief Constructs a JSON value storing an object (move).
	 * @param obj Object of key-value pairs to move into the new value.
	 *
	 * @details Enables efficient construction when the object is temporary.
	 */
	JSON(Object&& obj);

public:
	/**
	 * @brief Retrieves the type of the stored JSON value.
	 * @return A JSON::Type enumeration value indicating the current type.
	 */
	Type GetType() const;

	/**
	 * @brief Gets the stored boolean value.
	 * @return The boolean value.
	 * @throw std::runtime_error if the stored value is not a boolean.
	 */
	bool AsBool() const;

	/**
	 * @brief Gets the stored numeric value.
	 * @return The numeric value stored as a double.
	 * @throw std::runtime_error if the stored value is not a number.
	 */
	double AsNumber() const;

	/**
	 * @brief Gets the stored string value.
	 * @return A constant reference to the string.
	 * @throw std::runtime_error if the stored value is not a string.
	 */
	const std::string& AsString() const;

	/**
	 * @brief Gets the stored array.
	 * @return A constant reference to the array.
	 * @throw std::runtime_error if the stored value is not an array.
	 */
	const Array& AsArray() const;

	/**
	 * @brief Gets the stored object.
	 * @return A constant reference to the object.
	 * @throw std::runtime_error if the stored value is not an object.
	 */
	const Object& AsObject() const;

public:
	/**
	 * @brief Gets the underlying variant holding the JSON value.
	 * @return A constant reference to the variant.
	 *
	 * @details This is useful for low-level access or for implementing additional
	 * functionality without exposing implementation details.
	 */
	const Value& GetValue() const;

	/**
	 * @brief Template method to retrieve the stored value as a specific type.
	 * @tparam T The type to which the stored value should be converted.
	 * @return The stored value cast to type T.
	 * @throw std::runtime_error if the stored value cannot be converted to type T.
	 *
	 * @details This method is a convenient wrapper around std::get. It helps to ensure
	 * that the correct type is retrieved from the underlying variant.
	 */
	template<typename T>
	T Get() const
	{
		try
		{
			return std::get<T>(m_value);
		}
		catch (const std::bad_variant_access&)
		{
			throw std::runtime_error("JSON conversion error: type mismatch");
		}
	}

	/**
	 * @brief Returns the number of elements in a JSON array or object.
	 * @return The size (number of elements).
	 * @throw std::runtime_error if the stored value is neither an array nor an object.
	 */
	std::size_t Size() const;

	/**
	 * @brief Checks if a JSON object contains a specific key.
	 * @param key The key to search for.
	 * @return True if the object contains the key, false otherwise.
	 * @throw std::runtime_error if the stored value is not an object.
	 */
	bool Contains(const std::string& key) const;

public:
	/**
	 * @brief Read-only access to an object member by key.
	 * @param key The key whose associated value is to be retrieved.
	 * @return A constant reference to the JSON value associated with the specified key.
	 * @throw std::runtime_error if the JSON value is not an object or if the key does not exist.
	 *
	 * @details This operator allows you to conveniently access values in a JSON object.
	 */
	const JSON& operator[](const std::string& key) const;

	/**
	 * @brief Access or create an object member by key.
	 * @param key The key to access.
	 * @return A reference to the JSON value corresponding to the key.
	 * @throw std::runtime_error if the stored value is not an object.
	 *
	 * @details If the key does not exist in the object, this operator creates a new
	 * entry with a null value.
	 */
	JSON& operator[](const std::string& key);

	/**
	 * @brief Read-only access to an array element by index.
	 * @param index The index of the element.
	 * @return A constant reference to the JSON value at the specified index.
	 * @throw std::runtime_error if the stored value is not an array or if the index is out of bounds.
	 *
	 * @details This operator provides array-like access to JSON array values.
	 */
	const JSON& operator[](std::size_t index) const;

	/**
	 * @brief Access (modifiable) an array element by index.
	 * @param index The index of the element.
	 * @return A reference to the JSON value at the specified index.
	 * @throw std::runtime_error if the stored value is not an array or if the index is out of bounds.
	 */
	JSON& operator[](std::size_t index);

	/**
	 * @brief Conversion operator to bool.
	 *
	 * @details Use this operator to obtain a boolean representation of the JSON value.
	 */
	explicit operator bool() const { return AsBool(); }

	/**
	 * @brief Conversion operator to double.
	 *
	 * @details Use this operator to obtain a numeric representation of the JSON value.
	 */
	explicit operator double() const { return AsNumber(); }

	/**
	 * @brief Conversion operator to std::string.
	 *
	 * @details Use this operator to obtain a string representation of the JSON value.
	 */
	explicit operator std::string() const { return AsString(); }

	/**
	 * @brief Overloaded stream insertion operator for JSON values.
	 *
	 * @param os The output stream.
	 * @param j The JSON value to be output.
	 * @return The output stream.
	 *
	 * @details This operator provides a convenient way to print JSON values directly to
	 * an output stream (e.g., std::cout).
	 */
	friend std::ostream& operator<<(std::ostream& os, const JSON& j);

	/**
	 * @brief Stream extraction operator for JSON values.
	 *
	 * Reads a JSON-formatted string from an input stream and parses it into a JSON object.
	 *
	 * @param is The input stream.
	 * @param j The JSON object to which the parsed content is assigned.
	 * @return Reference to the input stream.
	 */
	friend std::istream& operator>>(std::istream& is, JSON& j);

public:
	/**
	 * @brief Returns a JSON-formatted string.
	 *
	 * If indent < 0 the output is compact, otherwise pretty-printed using the given
	 * indent character.
	 *
	 * @param indent Number of spaces for indentation (default: -1 for compact).
	 * @param indent_char The character to use for indentation (default: space).
	 * @return The JSON string.
	 */
	std::string dump(int indent = -1, char indent_char = ' ') const;

	/**
	 * @brief Parses a JSON-formatted string and returns a JSON object.
	 *
	 * This provides functionality similar to nlohmann::json::parse.
	 *
	 * @param input The JSON-formatted string.
	 * @return A JSON object.
	 * @throw std::runtime_error if parsing fails.
	 */
	static JSON parse(const std::string& input);

private:
	std::string Serialize(bool pretty, int indent, char indent_char) const;
	std::string SerializeArray(bool pretty, int indent, char indent_char) const;
	std::string SerializeObject(bool pretty, int indent, char indent_char) const;
	static std::string EscapeString(const std::string& input);

private:
	Value m_value; ///< Underlying storage for the JSON value.
};

} // namespace ISXJson
