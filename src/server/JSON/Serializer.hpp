
/**
 * @file Serializer.hpp
 * @brief Declaration of serialization and deserialization templates for JSON.
 *
 * @details
 * This header defines the Serializer template and its specializations for
 * converting between C++ types and JSON representation. It provides a
 * type-based conversion system that works with the JSON class.
 */

#pragma once

#include <string>
#include <vector>

namespace ISXJson
{
// Forward declaration of JSON class
class JSON;

/**
 * @brief Primary template for serializing and deserializing types to/from JSON.
 *
 * @details
 * This primary template provides a compile-time error if no specialization is found
 * for a given type. Each specialization should implement static Serialize and Deserialize
 * methods to convert between the type and JSON.
 *
 * @tparam T The C++ type to serialize/deserialize.
 */
template<typename T>
struct Serializer
{
	static_assert(sizeof(T) == 0, "No JSONSerializer specialization found for this type");

	// static JSON Serialize(const T& value);
	// static T Deserialize(const JSON& json);
};

/**
 * @brief Serializer specialization for std::string.
 *
 * @details
 * Handles conversion between std::string and JSON string value.
 */
template<>
struct Serializer<std::string>
{
	/**
	 * @brief Converts a std::string to a JSON value.
	 * @param value The string to convert.
	 * @return A JSON value containing the string.
	 */
	static JSON Serialize(const std::string& value);

	/**
	 * @brief Extracts a std::string from a JSON value.
	 * @param json The JSON value to convert.
	 * @return The extracted string.
	 * @throw std::runtime_error If the JSON value is not a string.
	 */
	static std::string Deserialize(const JSON& json);
};

/**
 * @brief Serializer specialization for int.
 *
 * @details
 * Handles conversion between int and JSON number value.
 */
template<>
struct Serializer<int>
{
	/**
	 * @brief Converts an int to a JSON value.
	 * @param value The integer to convert.
	 * @return A JSON value containing the number.
	 */
	static JSON Serialize(int value);

	/**
	 * @brief Extracts an int from a JSON value.
	 * @param json The JSON value to convert.
	 * @return The extracted integer.
	 * @throw std::runtime_error If the JSON value is not a number.
	 */
	static int Deserialize(const JSON& json);
};

/**
 * @brief Serializer specialization for bool.
 *
 * @details
 * Handles conversion between bool and JSON boolean value.
 */
template<>
struct Serializer<bool>
{
	/**
	 * @brief Converts a bool to a JSON value.
	 * @param value The boolean to convert.
	 * @return A JSON value containing the boolean.
	 */
	static JSON Serialize(bool value);

	/**
	 * @brief Extracts a bool from a JSON value.
	 * @param json The JSON value to convert.
	 * @return The extracted boolean.
	 * @throw std::runtime_error If the JSON value is not a boolean.
	 */
	static bool Deserialize(const JSON& json);
};

/**
 * @brief Serializer specialization for double.
 *
 * @details
 * Handles conversion between double and JSON number value.
 */
template<>
struct Serializer<double>
{
	/**
	 * @brief Converts a double to a JSON value.
	 * @param value The floating-point number to convert.
	 * @return A JSON value containing the number.
	 */
	static JSON Serialize(double value);

	/**
	 * @brief Extracts a double from a JSON value.
	 * @param json The JSON value to convert.
	 * @return The extracted floating-point number.
	 * @throw std::runtime_error If the JSON value is not a number.
	 */
	static double Deserialize(const JSON& json);
};

/**
 * @brief Serializer specialization for std::vector<T>.
 *
 * @details
 * Handles conversion between std::vector<T> and JSON array value.
 * Recursively applies serialization/deserialization to each element.
 *
 * @tparam T The type of elements in the vector.
 */
template<typename T>
struct Serializer<std::vector<T>>
{
	/**
	 * @brief Converts a vector to a JSON array.
	 * @param vec The vector to convert.
	 * @return A JSON array containing the serialized elements.
	 */
	static JSON Serialize(const std::vector<T>& vec);

	/**
	 * @brief Extracts a vector from a JSON array.
	 * @param json The JSON array to convert.
	 * @return A vector containing the deserialized elements.
	 * @throw std::runtime_error If the JSON value is not an array or
	 *                          if elements can't be converted to type T.
	 */
	static std::vector<T> Deserialize(const JSON& json);
};

} // namespace ISXJson
